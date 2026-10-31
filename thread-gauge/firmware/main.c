#include <stdbool.h>
#include <stdint.h>

/* Platform-independent calculation core. Target GPIO/USB adapter not included. */
bool thread_gauge_tex(uint32_t mass_mg, uint32_t length_mm, uint32_t *out_tex)
{
    if (!out_tex || mass_mg == 0U || length_mm == 0U) return false;
    /* tex = g/km; mg/mm has the same numeric scale. */
    *out_tex = (uint32_t)(((uint64_t)mass_mg * 1000U + length_mm / 2U) / length_mm);
    return true;
}

bool thread_gauge_denier(uint32_t mass_mg, uint32_t length_mm, uint32_t *out_denier)
{
    if (!out_denier || mass_mg == 0U || length_mm == 0U) return false;
    /* denier = g per 9 km = 9 * tex. */
    uint64_t numerator = (uint64_t)mass_mg * 9000U;
    *out_denier = (uint32_t)((numerator + length_mm / 2U) / length_mm);
    return true;
}
