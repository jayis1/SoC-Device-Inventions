/* Wheel Whisper portable Goertzel core. Designed by jayis1.
 * Reference-design code: host-tested; target SDK integration is not validated.
 */
#include "wheel_whisper.h"
#include <math.h>

static float goertzel_energy(const uint16_t *x, size_t n, float mean,
                             float frequency, uint32_t rate) {
    const float coefficient = 2.0f * cosf(2.0f * 3.14159265358979323846f * frequency / (float)rate);
    float s0 = 0.0f, s1 = 0.0f, s2 = 0.0f;
    for (size_t i = 0; i < n; ++i) {
        s0 = ((float)x[i] - mean) + coefficient * s1 - s2;
        s2 = s1;
        s1 = s0;
    }
    return s1 * s1 + s2 * s2 - coefficient * s1 * s2;
}

ww_measurement_t ww_analyze_capture(const uint16_t *samples, size_t count,
                                    uint32_t rate, float min_hz, float max_hz,
                                    float step_hz, uint16_t full_scale) {
    ww_measurement_t out = {0.0f, 0.0f, WW_CAPTURE_TOO_WEAK};
    if (!samples || count < 64 || rate == 0 || min_hz <= 0.0f ||
        max_hz <= min_hz || step_hz <= 0.0f) return out;
    float mean = 0.0f;
    uint16_t lo = full_scale, hi = 0;
    for (size_t i = 0; i < count; ++i) {
        mean += (float)samples[i];
        if (samples[i] < lo) lo = samples[i];
        if (samples[i] > hi) hi = samples[i];
    }
    mean /= (float)count;
    if (lo < 4 || hi > (uint16_t)(full_scale - 4)) {
        out.status = WW_CAPTURE_SATURATED;
        return out;
    }
    if ((uint16_t)(hi - lo) < 12) return out;

    float best = 0.0f, second = 0.0f, best_hz = 0.0f;
    for (float hz = min_hz; hz <= max_hz; hz += step_hz) {
        float e = goertzel_energy(samples, count, mean, hz, rate);
        if (e > best) { second = best; best = e; best_hz = hz; }
        else if (e > second) second = e;
    }
    if (best < 1000.0f) return out;
    out.frequency_hz = best_hz;
    out.quality = best / (second + 1.0f);
    out.status = out.quality < 1.15f ? WW_CAPTURE_AMBIGUOUS : WW_CAPTURE_OK;
    return out;
}
