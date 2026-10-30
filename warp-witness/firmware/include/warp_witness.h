#ifndef WARP_WITNESS_H
#define WARP_WITNESS_H
#include <stdbool.h>
#include <stdint.h>
#define WW_MAX_LENGTH_MM 10000u
#define WW_MIN_LENGTH_MM 10u
#define WW_MAX_MASS_MG 1000000u
bool ww_tex_milli(uint32_t mass_mg, uint32_t length_mm, uint32_t *tex_milli);
bool ww_turns_per_m(uint32_t turns, uint32_t length_mm, uint32_t *turns_per_milli);
#endif
