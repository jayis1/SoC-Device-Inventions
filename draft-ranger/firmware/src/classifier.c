#include "classifier.h"
#include "psychrometrics.h"
#include "acoustics.h"

static double clamp(double value, double lo, double hi)
{
    if (value < lo) {
        return lo;
    }
    if (value > hi) {
        return hi;
    }
    return value;
}

const char *dr_fault_class_name(dr_fault_class_t fault_class)
{
    switch (fault_class) {
    case DR_SEAL_OK:
        return "SEAL_OK";
    case DR_MINOR_WEATHERSTRIP_GAP:
        return "MINOR_WEATHERSTRIP_GAP";
    case DR_PRESSURE_DRIVEN_DRAFT:
        return "PRESSURE_DRIVEN_DRAFT";
    case DR_WARPED_FRAME_OR_SASH:
        return "WARPED_FRAME_OR_SASH";
    case DR_CONDENSATION_RISK:
        return "CONDENSATION_RISK";
    case DR_CRITICAL_COMBINED:
        return "CRITICAL_COMBINED";
    default:
        return "UNKNOWN";
    }
}

void dr_classify_capture(const dr_capture_t *capture, dr_result_t *result)
{
    dr_acoustic_metrics_t acoustic = {0};
    double pressure_score;
    double velocity_score;
    double condensation_penalty;
    double whistle_bonus;

    result->dew_point_c = dr_compute_dew_point_c(capture->ambient_temp_c, capture->ambient_rh_pct);
    result->condensation_margin_c = dr_compute_condensation_margin_c(capture, result->dew_point_c);
    result->draft_velocity_mps = dr_compute_velocity_mps(capture->pressure_pa, capture->bridge_v);

    dr_analyze_acoustics(capture, &acoustic);
    result->whistle_hz = acoustic.dominant_hz;
    result->whistle_score = acoustic.whistle_score;

    pressure_score = clamp(capture->pressure_pa / 30.0, 0.0, 1.0);
    velocity_score = clamp(result->draft_velocity_mps / 3.2, 0.0, 1.0);
    condensation_penalty = clamp((1.0 - result->condensation_margin_c) / 4.0, 0.0, 1.0);
    whistle_bonus = clamp((result->whistle_score - 0.18) / 0.45, 0.0, 1.0);

    result->leak_severity = 100.0 * clamp(0.35 * pressure_score
                                        + 0.35 * velocity_score
                                        + 0.15 * whistle_bonus
                                        + 0.15 * condensation_penalty,
                                          0.0, 1.0);
    result->seal_health = 100.0 - result->leak_severity;

    if (result->leak_severity < 18.0 && result->condensation_margin_c > 4.0) {
        result->fault_class = DR_SEAL_OK;
    } else if (result->condensation_margin_c < 0.5 && result->leak_severity < 45.0) {
        result->fault_class = DR_CONDENSATION_RISK;
    } else if (result->condensation_margin_c < 0.0 && result->leak_severity >= 45.0) {
        result->fault_class = DR_CRITICAL_COMBINED;
    } else if (capture->pressure_pa >= 20.0 && result->draft_velocity_mps >= 2.0 && result->whistle_score < 0.30) {
        result->fault_class = DR_PRESSURE_DRIVEN_DRAFT;
    } else if (result->whistle_hz >= 1800.0 && result->whistle_score >= 0.30 && result->draft_velocity_mps >= 1.2) {
        result->fault_class = DR_WARPED_FRAME_OR_SASH;
    } else {
        result->fault_class = DR_MINOR_WEATHERSTRIP_GAP;
    }
}
