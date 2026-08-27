#include <math.h>
#include <stddef.h>

#include "classifier.h"
#include "line_db.h"
#include "tdr.h"

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

const char *ls_fault_name(ls_fault_t fault)
{
    switch (fault) {
    case LS_OPEN_END:
        return "OPEN_END";
    case LS_SHORT_END:
        return "SHORT_END";
    case LS_HIGH_RESISTANCE_SPLICE:
        return "HIGH_RESISTANCE_SPLICE";
    case LS_WATER_INGRESS:
        return "WATER_INGRESS";
    case LS_BRIDGE_TAP:
        return "BRIDGE_TAP";
    case LS_SPLIT_PAIR:
        return "SPLIT_PAIR";
    case LS_HEALTHY_LINE:
        return "HEALTHY_LINE";
    case LS_OVERVOLTAGE_LOCKOUT:
        return "OVERVOLTAGE_LOCKOUT";
    default:
        return "UNKNOWN_FAULT";
    }
}

void ls_classify_capture(const ls_capture_t *capture, ls_result_t *result)
{
    const float distance_m = ls_estimate_distance_m(capture);
    const float primary_v = ls_primary_reflection_v(capture);
    const float expected_cap_nf = ls_expected_capacitance_nf_for_type(capture->cable_type);
    const float cap_excess = capture->cap_nf - expected_cap_nf;
    const float impedance_hint = ls_impedance_hint_ohm(capture);
    float health = 100.0f;
    ls_fault_t fault = LS_UNKNOWN_FAULT;
    const char *notes = "inspect manually";
    float confidence = 0.55f;

    if (fabsf(capture->foreign_voltage_v) > 18.0f) {
        fault = LS_OVERVOLTAGE_LOCKOUT;
        health = 0.0f;
        confidence = 0.99f;
        notes = "foreign voltage present; do not launch pulse";
    } else if ((capture->secondary_ns > capture->round_trip_ns + 15.0f) && (capture->refl_peak_v > 0.40f)) {
        fault = LS_BRIDGE_TAP;
        health -= 52.0f;
        health -= 25.0f * capture->ringing_ratio;
        confidence = 0.86f;
        notes = "double positive event suggests an unterminated branch";
    } else if ((capture->pair_balance_pct < 78.0f) && (capture->loop_res_ohm < 30.0f) && (fabsf(primary_v) < 0.30f)) {
        fault = LS_SPLIT_PAIR;
        health -= 36.0f;
        health -= 0.35f * (78.0f - capture->pair_balance_pct);
        confidence = 0.82f;
        notes = "continuity okay, but balance/crosstalk proxy is poor";
    } else if ((capture->refl_trough_v < -0.32f) && (cap_excess > 2.5f) && (capture->pair_balance_pct < 75.0f)) {
        fault = LS_WATER_INGRESS;
        health -= 58.0f;
        health -= 4.0f * cap_excess;
        confidence = 0.90f;
        notes = "broad negative reflection with elevated capacitance";
    } else if ((capture->refl_peak_v > 0.60f) && (capture->loop_res_ohm > 40.0f)) {
        fault = LS_OPEN_END;
        health -= 42.0f;
        health -= 0.10f * distance_m;
        confidence = 0.88f;
        notes = "strong positive reflection indicates open or severed conductor";
    } else if ((capture->refl_trough_v < -0.60f) && (capture->loop_res_ohm < 10.0f)) {
        fault = LS_SHORT_END;
        health -= 65.0f;
        confidence = 0.91f;
        notes = "strong negative reflection and low loop resistance";
    } else if ((capture->refl_peak_v > 0.22f) && (capture->loop_res_ohm > 35.0f)) {
        fault = LS_HIGH_RESISTANCE_SPLICE;
        health -= 45.0f;
        health -= 0.20f * (capture->loop_res_ohm - 35.0f);
        confidence = 0.78f;
        notes = "moderate positive reflection with elevated loop resistance";
    } else if ((fabsf(primary_v) < 0.10f) && (capture->pair_balance_pct > 95.0f)) {
        fault = LS_HEALTHY_LINE;
        health = 92.0f - 10.0f * capture->ringing_ratio;
        confidence = 0.72f;
        notes = "reflection near matched endpoint";
    } else if (primary_v > 0.0f) {
        fault = LS_OPEN_END;
        health -= 35.0f;
        confidence = 0.60f;
        notes = "positive reflection, weak confidence";
    } else {
        fault = LS_SHORT_END;
        health -= 40.0f;
        confidence = 0.60f;
        notes = "negative reflection, weak confidence";
    }

    result->fault = fault;
    result->est_distance_m = distance_m;
    result->health_score = clampf(health, 0.0f, 100.0f);
    result->confidence = confidence;
    result->impedance_hint_ohm = impedance_hint;
    result->notes = notes;
}
