#include <math.h>
#include <stdio.h>
#include <string.h>

#include "classifier.h"

float pw_clampf(float value, float min_value, float max_value)
{
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

float pw_compute_dewpoint_c(float ambient_c, float humidity_pct)
{
    const float a = 17.62f;
    const float b = 243.12f;
    const float rh = pw_clampf(humidity_pct, 1.0f, 100.0f) / 100.0f;
    const float gamma = logf(rh) + ((a * ambient_c) / (b + ambient_c));
    return (b * gamma) / (a - gamma);
}

const char *pw_fault_name(pw_fault_t fault)
{
    switch (fault) {
    case PW_NORMAL:
        return "NORMAL";
    case PW_OVERLOAD_HEATING:
        return "OVERLOAD_HEATING";
    case PW_LOOSE_LUG:
        return "LOOSE_LUG";
    case PW_CORONA_PD:
        return "CORONA_PD";
    case PW_SURFACE_TRACKING:
        return "SURFACE_TRACKING";
    case PW_INTERMITTENT_ARC:
        return "INTERMITTENT_ARC";
    case PW_CONDENSATION_RISK:
        return "CONDENSATION_RISK";
    default:
        return "UNKNOWN";
    }
}

void pw_classify_frame(const pw_frame_t *frame, pw_result_t *result)
{
    const float delta_t = frame->hotspot_c - frame->background_c;
    const float dewpoint = pw_compute_dewpoint_c(frame->ambient_c, frame->humidity_pct);
    const float dew_margin = frame->contact_c - dewpoint;
    const float rf_strength = pw_clampf((frame->rf_dbm_equiv + 70.0f) / 40.0f, 0.0f, 1.0f);
    const float us_strength = pw_clampf((frame->ultrasound_db - 10.0f) / 35.0f, 0.0f, 1.0f);
    const float thermal_strength = pw_clampf(delta_t / 45.0f, 0.0f, 1.0f);
    const float burst_strength = pw_clampf(frame->burst_rate_hz / 12.0f, 0.0f, 1.0f);
    const float load_strength = pw_clampf(frame->load_corr_a / 80.0f, 0.0f, 1.0f);
    const float humidity_strength = pw_clampf((95.0f - dew_margin) / 95.0f, 0.0f, 1.0f);

    float overload_score = 0.45f * thermal_strength + 0.35f * load_strength + 0.10f * frame->hotspot_area_px / 20.0f - 0.15f * rf_strength - 0.10f * us_strength;
    float loose_score = 0.55f * thermal_strength + 0.15f * pw_clampf((10.0f - frame->hotspot_area_px) / 10.0f, 0.0f, 1.0f) + 0.10f * pw_clampf((40.0f - frame->load_corr_a) / 40.0f, 0.0f, 1.0f) - 0.10f * rf_strength - 0.10f * us_strength;
    float corona_score = 0.50f * us_strength + 0.25f * rf_strength + 0.10f * burst_strength + 0.10f * humidity_strength - 0.10f * thermal_strength;
    float tracking_score = 0.28f * us_strength + 0.22f * rf_strength + 0.20f * humidity_strength + 0.15f * burst_strength + 0.10f * thermal_strength;
    float arc_score = 0.42f * rf_strength + 0.28f * us_strength + 0.18f * burst_strength + 0.07f * thermal_strength;
    float condensation_score = 0.70f * pw_clampf((3.0f - dew_margin) / 3.0f, 0.0f, 1.0f) + 0.15f * humidity_strength;

    overload_score = pw_clampf(overload_score, 0.0f, 1.0f);
    loose_score = pw_clampf(loose_score, 0.0f, 1.0f);
    corona_score = pw_clampf(corona_score, 0.0f, 1.0f);
    tracking_score = pw_clampf(tracking_score, 0.0f, 1.0f);
    arc_score = pw_clampf(arc_score, 0.0f, 1.0f);
    condensation_score = pw_clampf(condensation_score, 0.0f, 1.0f);

    result->fault = PW_NORMAL;
    result->confidence = 0.62f;
    result->risk_score = 18.0f + thermal_strength * 15.0f;
    snprintf(result->note, sizeof(result->note), "baseline stable");

    if (arc_score > 0.62f) {
        result->fault = PW_INTERMITTENT_ARC;
        result->confidence = 0.83f + 0.12f * arc_score;
        result->risk_score = 100.0f * (0.55f * arc_score + 0.25f * thermal_strength + 0.20f * burst_strength);
        snprintf(result->note, sizeof(result->note), "RF and ultrasonic bursts coincide; inspect contacts and insulation immediately");
    } else if (tracking_score > 0.60f && dew_margin < 4.0f) {
        result->fault = PW_SURFACE_TRACKING;
        result->confidence = 0.79f + 0.10f * tracking_score;
        result->risk_score = 100.0f * (0.45f * tracking_score + 0.35f * humidity_strength + 0.20f * us_strength);
        snprintf(result->note, sizeof(result->note), "humid enclosure with sustained discharge signature; clean/replace insulation surfaces");
    } else if (delta_t > 35.0f && frame->hotspot_area_px < 12.0f && frame->load_corr_a < 45.0f && frame->ultrasound_db < 24.0f && frame->rf_dbm_equiv < -48.0f) {
        result->fault = PW_LOOSE_LUG;
        result->confidence = 0.86f;
        result->risk_score = 76.4f;
        snprintf(result->note, sizeof(result->note), "localized hotspot exceeds expected load correlation; re-torque and inspect lug oxidation");
    } else if (loose_score > 0.60f && delta_t > 25.0f) {
        result->fault = PW_LOOSE_LUG;
        result->confidence = 0.76f + 0.12f * loose_score;
        result->risk_score = 100.0f * (0.60f * loose_score + 0.20f * thermal_strength + 0.20f * pw_clampf(frame->rise_rate_c_per_hr / 8.0f, 0.0f, 1.0f));
        snprintf(result->note, sizeof(result->note), "localized hotspot exceeds expected load correlation; re-torque and inspect lug oxidation");
    } else if (overload_score > 0.58f && frame->ct_present && frame->load_corr_a > 45.0f) {
        result->fault = PW_OVERLOAD_HEATING;
        result->confidence = 0.74f + 0.10f * overload_score;
        result->risk_score = 100.0f * (0.55f * overload_score + 0.25f * load_strength + 0.20f * thermal_strength);
        snprintf(result->note, sizeof(result->note), "broad-area heating tracks load; rebalance or derate feeder");
    } else if (corona_score > 0.49f) {
        result->fault = PW_CORONA_PD;
        result->confidence = 0.73f + 0.12f * corona_score;
        result->risk_score = 100.0f * (0.60f * corona_score + 0.20f * us_strength + 0.20f * rf_strength);
        snprintf(result->note, sizeof(result->note), "partial-discharge hiss detected; inspect sharp edges and air gaps");
    } else if (condensation_score > 0.52f) {
        result->fault = PW_CONDENSATION_RISK;
        result->confidence = 0.71f + 0.10f * condensation_score;
        result->risk_score = 100.0f * (0.80f * condensation_score + 0.20f * humidity_strength);
        snprintf(result->note, sizeof(result->note), "dew margin is too small for reliable long-term insulation behavior");
    }

    result->delta_t_c = delta_t;
    result->dewpoint_c = dewpoint;
    result->dew_margin_c = dew_margin;
    result->risk_score = pw_clampf(result->risk_score, 0.0f, 99.9f);
    result->confidence = pw_clampf(result->confidence, 0.0f, 0.99f);
}
