#include <stdint.h>

typedef enum {
    FUSE_OPEN = 0,
    FUSE_CHECK = 1,
    FUSE_PASS = 2
} fuse_status_t;

/* Classifies an averaged 12-bit RP2040 ADC reading. Thresholds are provisional. */
fuse_status_t fuse_scout_classify(uint16_t adc_counts)
{
    if (adc_counts > 4095U) {
        return FUSE_OPEN;
    }
    if (adc_counts < 248U) { /* 0.4 V at fixture node through 2:1 divider */
        return FUSE_OPEN;
    }
    if (adc_counts < 1241U) { /* 2.0 V at fixture node through 2:1 divider */
        return FUSE_CHECK;
    }
    return FUSE_PASS;
}

uint16_t fuse_scout_average(const uint16_t *samples, uint32_t count)
{
    uint32_t sum = 0U;
    uint32_t i;
    if (samples == 0 || count == 0U) {
        return 0U;
    }
    for (i = 0U; i < count; ++i) {
        sum += (samples[i] > 4095U) ? 4095U : samples[i];
    }
    return (uint16_t)(sum / count);
}
