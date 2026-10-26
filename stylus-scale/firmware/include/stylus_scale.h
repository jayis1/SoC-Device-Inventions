#ifndef STYLUS_SCALE_H
#define STYLUS_SCALE_H
#include <stdint.h>
#define SS_FILTER_N 5U
typedef struct { int32_t samples[SS_FILTER_N]; uint8_t count, next; } ss_filter_t;
void ss_filter_init(ss_filter_t *filter);
void ss_filter_push(ss_filter_t *filter, int32_t sample);
int32_t ss_filter_value(const ss_filter_t *filter);
float ss_grams(int32_t raw, int32_t zero, float counts_per_gram);
#endif
