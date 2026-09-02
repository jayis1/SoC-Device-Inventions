#include <math.h>

#include "dsp.h"

static float clampf(float value, float low, float high)
{
    if (value < low) {
        return low;
    }
    if (value > high) {
        return high;
    }
    return value;
}

static float dew_point_c(float temp_c, float rh)
{
    const float a = 17.62f;
    const float b = 243.12f;
    float gamma = logf(fmaxf(rh, 0.1f) / 100.0f) + (a * temp_c) / (b + temp_c);
    return (b * gamma) / (a - gamma);
}

void ms_extract_features(const ms_sensor_window_t *window, ms_features_t *features)
{
    float dp_c = dew_point_c(window->ambient_temp_c, window->ambient_rh);
    float phase_abs = fabsf(window->phase_deg);
    float thermal_abs = fabsf(window->thermal_delta_c);

    features->dewpoint_margin_c = window->surface_temp_c - dp_c;
    features->moisture_proxy = clampf((55.0f - window->impedance_kohm) / 30.0f + phase_abs / 90.0f, 0.0f, 1.0f);
    features->condensation_risk = clampf((1.5f - features->dewpoint_margin_c) / 3.5f, 0.0f, 1.0f);
    features->thermal_bridge_score = clampf(thermal_abs / 6.0f, 0.0f, 1.0f);
    features->voc_risk = clampf((window->voc_index - 100.0f) / 180.0f + (30.0f - window->gas_resistance_kohm) / 60.0f, 0.0f, 1.0f);
    features->mold_risk_score = clampf(
        0.45f * features->moisture_proxy +
        0.30f * features->condensation_risk +
        0.15f * features->thermal_bridge_score +
        0.10f * features->voc_risk,
        0.0f,
        1.0f);
}
