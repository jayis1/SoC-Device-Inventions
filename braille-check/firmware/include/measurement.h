#ifndef BRAILLE_CHECK_MEASUREMENT_H
#define BRAILLE_CHECK_MEASUREMENT_H
#include <stddef.h>
#include <stdint.h>
#define BC_DOT_COUNT 6
struct bc_result { int32_t height_um[BC_DOT_COUNT]; int32_t min_um, max_um; int64_t mean_um; };
/* Inputs are externally calibrated displacement readings; returns 0 on success. */
int bc_measure(const int32_t base_um[BC_DOT_COUNT], const int32_t top_um[BC_DOT_COUNT], struct bc_result *out);
#endif
