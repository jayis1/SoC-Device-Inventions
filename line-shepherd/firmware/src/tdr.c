#include <math.h>

#include "tdr.h"

static const float SPEED_OF_LIGHT_M_PER_S = 299792458.0f;

float ls_estimate_distance_m(const ls_capture_t *capture)
{
    if ((capture == 0) || (capture->round_trip_ns <= 0.0f) || (capture->velocity_factor <= 0.0f)) {
        return 0.0f;
    }

    return 0.5f * (capture->round_trip_ns * 1.0e-9f) * SPEED_OF_LIGHT_M_PER_S * capture->velocity_factor;
}

float ls_primary_reflection_v(const ls_capture_t *capture)
{
    const float pos = fabsf(capture->refl_peak_v);
    const float neg = fabsf(capture->refl_trough_v);
    return (pos >= neg) ? capture->refl_peak_v : capture->refl_trough_v;
}

float ls_impedance_hint_ohm(const ls_capture_t *capture)
{
    const float gamma = ls_primary_reflection_v(capture);
    const float source_impedance = 100.0f;

    if (gamma >= 0.98f) {
        return 10000.0f;
    }
    if (gamma <= -0.98f) {
        return 0.5f;
    }

    return source_impedance * (1.0f + gamma) / (1.0f - gamma);
}
