#include "paper_bias.h"
#include <limits.h>

void pb_reset(pb_accumulator_t *acc) {
    if (acc) { acc->sum = 0; acc->samples = 0; }
}

bool pb_add(pb_accumulator_t *acc, uint16_t adc, uint16_t *mean_out) {
    if (!acc || !mean_out || adc > 4095 || acc->samples == UINT32_MAX) return false;
    acc->sum += adc;
    ++acc->samples;
    *mean_out = (uint16_t)(acc->sum / acc->samples);
    return true;
}

bool pb_directional_ratio(uint16_t a, uint16_t b, uint16_t deadband, uint32_t *ratio_milli) {
    if (!ratio_milli || a <= deadband || b <= deadband) return false;
    uint32_t high = a > b ? a : b;
    uint32_t low = a > b ? b : a;
    *ratio_milli = (high * 1000u) / low;
    return true;
}
