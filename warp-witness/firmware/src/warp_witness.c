#include "warp_witness.h"
#include <limits.h>

bool ww_tex_milli(uint32_t mass_mg, uint32_t length_mm, uint32_t *out) {
    if (out == 0 || length_mm < WW_MIN_LENGTH_MM || length_mm > WW_MAX_LENGTH_MM || mass_mg > WW_MAX_MASS_MG) return false;
    uint64_t value = (uint64_t)mass_mg * 1000u / length_mm;
    if (value > UINT32_MAX) return false;
    *out = (uint32_t)value;
    return true;
}

bool ww_turns_per_m(uint32_t turns, uint32_t length_mm, uint32_t *out) {
    if (out == 0 || length_mm < WW_MIN_LENGTH_MM || length_mm > WW_MAX_LENGTH_MM) return false;
    uint64_t value = (uint64_t)turns * 1000000u / length_mm;
    if (value > UINT32_MAX) return false;
    *out = (uint32_t)value;
    return true;
}
