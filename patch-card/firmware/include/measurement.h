#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct { float last_mm; float sum_mm; float min_mm; float max_mm; uint32_t accepted; uint32_t rejected; bool have_last; } pitch_stats_t;
void pitch_init(pitch_stats_t *s);
bool pitch_mark(pitch_stats_t *s, float travel_mm);
float pitch_mean(const pitch_stats_t *s);
