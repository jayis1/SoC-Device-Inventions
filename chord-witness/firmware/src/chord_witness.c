#include <math.h>
#include <stddef.h>
#include <stdint.h>

float cw_estimate_frequency(const uint16_t *samples, size_t count,
                            float sample_rate_hz, uint16_t midpoint,
                            uint16_t min_peak_to_peak)
{
    if (samples == NULL || count < 8 || sample_rate_hz <= 0.0f) return 0.0f;
    uint16_t lo = UINT16_MAX, hi = 0;
    for (size_t i = 0; i < count; ++i) {
        if (samples[i] > 4095u) return 0.0f;
        if (samples[i] < lo) lo = samples[i];
        if (samples[i] > hi) hi = samples[i];
    }
    if ((uint16_t)(hi - lo) < min_peak_to_peak) return 0.0f;
    uint32_t crossings = 0, first = 0, previous = 0;
    for (size_t i = 1; i < count; ++i) {
        if (samples[i - 1] < midpoint && samples[i] >= midpoint) {
            if (crossings == 0) first = (uint32_t)i;
            previous = (uint32_t)i;
            ++crossings;
        }
    }
    if (crossings < 2 || previous <= first) return 0.0f;
    float period_samples = (float)(previous - first) / (float)(crossings - 1);
    float frequency = sample_rate_hz / period_samples;
    return (frequency >= 40.0f && frequency <= 1500.0f) ? frequency : 0.0f;
}
