#pragma once
#include <stdbool.h>
#include <stdint.h>
typedef struct { float low_mm, high_mm; uint32_t sum; uint8_t count; } fm_state;
void fm_init(fm_state *s, float low_mm, float high_mm);
void fm_push(fm_state *s, uint16_t raw);
bool fm_reading(const fm_state *s, uint16_t *raw, float *mm);
