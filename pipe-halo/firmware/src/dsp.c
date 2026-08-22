#include <math.h>

#include "dsp.h"

static float clamp01(float x)
{
    if (x < 0.0f) {
        return 0.0f;
    }
    if (x > 1.0f) {
        return 1.0f;
    }
    return x;
}

void ph_extract_features(const ph_sensor_window_t *window, ph_features_t *features)
{
    float temp_gradient = fabsf(window->pipe_temp_upstream_c - window->pipe_temp_downstream_c);
    float acoustic_term = window->acoustic_rms_mv / 180.0f;
    float vibration_term = window->vibration_rms_g / 0.25f;
    float gradient_term = temp_gradient / 4.0f;
    float centroid_term = window->spectral_centroid_hz / 1200.0f;

    features->flow_index = clamp01(0.42f * acoustic_term +
                                   0.28f * vibration_term +
                                   0.20f * gradient_term +
                                   0.10f * centroid_term);

    features->hammer_index = clamp01((window->pressure_swing_kpa / 4.0f) * 0.7f +
                                     (window->vibration_rms_g / 0.22f) * 0.3f);

    if (window->drip_period_s > 0.2f && window->drip_period_s < 3.0f) {
        float periodicity = 1.0f - fabsf(window->drip_period_s - 0.9f) / 0.9f;
        features->drip_confidence = clamp01(periodicity * (window->acoustic_rms_mv / 60.0f));
    } else {
        features->drip_confidence = 0.0f;
    }

    {
        float min_pipe = window->pipe_temp_upstream_c < window->pipe_temp_downstream_c ?
                         window->pipe_temp_upstream_c : window->pipe_temp_downstream_c;
        features->freeze_margin_c = min_pipe - 0.5f;
    }

    {
        float freeze_severity = clamp01((0.5f - features->freeze_margin_c) / 2.5f);
        float humidity_term = clamp01((window->humidity_rh - 55.0f) / 35.0f);
        float valve_restriction = clamp01((75.0f - window->valve_angle_deg) / 45.0f);
        features->burst_risk_score = clamp01(0.45f * freeze_severity +
                                             0.25f * features->hammer_index +
                                             0.15f * humidity_term +
                                             0.15f * valve_restriction);
    }
}
