#include <stddef.h>

#include "classifier.h"
#include "dsp.h"
#include "thermals.h"

const char *ts_fault_class_name(ts_fault_class_t fault_class)
{
    switch (fault_class) {
    case TS_NORMAL_CYCLING:
        return "NORMAL_CYCLING";
    case TS_BLOW_THROUGH:
        return "BLOW_THROUGH";
    case TS_COLD_BLOCKED:
        return "COLD_BLOCKED";
    case TS_FLOODING:
        return "FLOODING";
    case TS_CAVITATING_VALVE:
        return "CAVITATING_VALVE";
    case TS_SENSOR_MISPLACED:
    default:
        return "SENSOR_MISPLACED";
    }
}

static double clamp01(double value)
{
    if (value < 0.0) {
        return 0.0;
    }
    if (value > 1.0) {
        return 1.0;
    }
    return value;
}

void ts_classify_capture(const ts_capture_t *capture, ts_result_t *result)
{
    if ((capture == NULL) || (result == NULL)) {
        return;
    }

    result->upstream_superheat_c = ts_compute_upstream_superheat_c(capture->upstream_temp_c,
                                                                   capture->ambient_temp_c);
    result->delta_t_c = ts_compute_delta_t_c(capture->upstream_temp_c,
                                             capture->downstream_temp_c);
    result->ultrasonic_rms = ts_compute_rms(capture->ultrasonic, TS_ENVELOPE_POINTS);
    result->crest_factor = ts_compute_crest_factor(capture->ultrasonic, TS_ENVELOPE_POINTS);
    result->burstiness = ts_compute_burstiness(capture->ultrasonic, TS_ENVELOPE_POINTS);
    result->active_fraction = ts_compute_active_fraction(capture->ultrasonic,
                                                         TS_ENVELOPE_POINTS,
                                                         0.45);
    result->cycle_rate_cpm = ts_estimate_cycle_rate_cpm(capture->ultrasonic,
                                                        TS_ENVELOPE_POINTS,
                                                        0.5);

    double health = 1.0;
    health -= 0.35 * clamp01(result->ultrasonic_rms);
    health -= 0.20 * clamp01(result->active_fraction);
    health -= 0.15 * clamp01((40.0 - result->delta_t_c) / 40.0);
    health -= 0.15 * clamp01((0.60 - result->burstiness) / 0.60);
    health -= 0.15 * clamp01((10.0 - capture->clamp_force_n) / 10.0);
    result->health_score = 100.0 * clamp01(health);

    if (capture->clamp_force_n < 8.0) {
        result->fault_class = TS_SENSOR_MISPLACED;
    } else if ((capture->upstream_temp_c < 110.0) &&
               (result->ultrasonic_rms > 0.55) &&
               (result->delta_t_c < 10.0) &&
               (result->active_fraction > 0.80)) {
        result->fault_class = TS_CAVITATING_VALVE;
    } else if ((result->ultrasonic_rms > 0.72) &&
               (result->delta_t_c < 20.0) &&
               (result->active_fraction > 0.80)) {
        result->fault_class = TS_BLOW_THROUGH;
    } else if ((result->delta_t_c > 70.0) &&
               (capture->downstream_temp_c < 70.0) &&
               (result->ultrasonic_rms < 0.12)) {
        result->fault_class = TS_COLD_BLOCKED;
    } else if ((result->burstiness > 0.65) &&
               (result->cycle_rate_cpm > 4.0) &&
               (result->cycle_rate_cpm < 12.0) &&
               (result->delta_t_c > 45.0)) {
        result->fault_class = TS_NORMAL_CYCLING;
    } else if ((result->delta_t_c > 15.0) &&
               (result->delta_t_c < 45.0) &&
               (result->ultrasonic_rms > 0.28) &&
               (result->active_fraction > 0.35)) {
        result->fault_class = TS_FLOODING;
    } else {
        result->fault_class = TS_SENSOR_MISPLACED;
    }
}
