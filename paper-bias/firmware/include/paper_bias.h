#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint64_t sum;
    uint32_t samples;
} pb_accumulator_t;

void pb_reset(pb_accumulator_t *acc);
bool pb_add(pb_accumulator_t *acc, uint16_t adc, uint16_t *mean_out);
bool pb_directional_ratio(uint16_t a, uint16_t b, uint16_t deadband, uint32_t *ratio_milli);
