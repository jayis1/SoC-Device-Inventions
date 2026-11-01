#include <stddef.h>
#include <stdint.h>
float cw_estimate_frequency(const uint16_t *samples, size_t count,
                            float sample_rate_hz, uint16_t midpoint,
                            uint16_t min_peak_to_peak);
