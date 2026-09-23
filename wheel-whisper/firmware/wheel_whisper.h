/* Wheel Whisper portable analysis contract. Designed by jayis1.
 * Reference-design code: host-tested; target SDK integration is not validated.
 */
#ifndef WHEEL_WHISPER_H
#define WHEEL_WHISPER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    WW_CAPTURE_OK = 0,
    WW_CAPTURE_TOO_WEAK,
    WW_CAPTURE_SATURATED,
    WW_CAPTURE_AMBIGUOUS
} ww_capture_status_t;

typedef struct {
    float frequency_hz;
    float quality;
    ww_capture_status_t status;
} ww_measurement_t;

/* Samples are unsigned ADC readings. The input must be a contiguous capture. */
ww_measurement_t ww_analyze_capture(const uint16_t *samples, size_t count,
                                    uint32_t sample_rate_hz,
                                    float min_hz, float max_hz,
                                    float step_hz, uint16_t adc_full_scale);

#endif
