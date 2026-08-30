#include <math.h>

#include "pulse_model.h"

void pp_extract_features(const pp_frame_t *frame, pp_features_t *features)
{
    const double safe_peak = frame->pulse_peak_kv > 0.05 ? frame->pulse_peak_kv : 0.05;
    const double baseline_interval = 1.0;
    const double humidity_term = frame->humidity_rh / 100.0;
    const double dew_term = frame->dewpoint_margin_c < 0.5
        ? 1.0
        : 1.0 / (1.0 + frame->dewpoint_margin_c);

    features->energy_index = safe_peak * frame->current_peak_a * (frame->decay_time_us / 100.0);
    features->leakage_ratio = frame->current_peak_a / safe_peak;
    features->wetness_factor = humidity_term * 0.7 + dew_term * 0.3;
    features->interval_error_pct = fabs(frame->pulse_interval_s - baseline_interval) * 100.0;

    features->fence_score = 100.0;
    features->fence_score -= fmax(0.0, (6.5 - frame->pulse_peak_kv) * 12.0);
    features->fence_score -= fmax(0.0, (features->leakage_ratio - 0.42) * 45.0);
    features->fence_score -= fmax(0.0, (features->interval_error_pct - 3.0) * 1.2);
    features->fence_score -= fmax(0.0, (3.10 - frame->battery_v) * 55.0);

    if (frame->pulse_missing) {
        features->fence_score = 0.0;
    }
    if (frame->surge_flag) {
        features->fence_score -= 12.0;
    }
    if (features->fence_score < 0.0) {
        features->fence_score = 0.0;
    }
}
