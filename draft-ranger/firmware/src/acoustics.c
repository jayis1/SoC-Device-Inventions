#include <math.h>
#include <stddef.h>

#include "acoustics.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static double goertzel_power(const double *samples, size_t count, double sample_rate, double target_hz)
{
    double coeff = 2.0 * cos(2.0 * M_PI * target_hz / sample_rate);
    double q0 = 0.0;
    double q1 = 0.0;
    double q2 = 0.0;

    for (size_t i = 0; i < count; ++i) {
        q0 = coeff * q1 - q2 + samples[i];
        q2 = q1;
        q1 = q0;
    }

    return q1 * q1 + q2 * q2 - coeff * q1 * q2;
}

void dr_analyze_acoustics(const dr_capture_t *capture, dr_acoustic_metrics_t *metrics)
{
    const double sample_rate = 16000.0;
    const double bins[] = {400.0, 800.0, 1200.0, 1800.0, 2600.0, 3600.0, 5200.0};
    const size_t bin_count = sizeof(bins) / sizeof(bins[0]);
    double total_power = 1e-9;
    double best_power = -1.0;
    double best_hz = 0.0;

    for (size_t i = 0; i < bin_count; ++i) {
        double power = goertzel_power(capture->audio, DR_AUDIO_SAMPLES, sample_rate, bins[i]);
        total_power += power;
        if (power > best_power) {
            best_power = power;
            best_hz = bins[i];
        }
    }

    metrics->dominant_hz = best_hz;
    metrics->whistle_score = best_power / total_power;
}
