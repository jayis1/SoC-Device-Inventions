#ifndef CLOCK_RATE_H
#define CLOCK_RATE_H
#include <stdint.h>
typedef struct { uint64_t last_us; uint32_t count; uint8_t armed; } cr_state_t;
void cr_init(cr_state_t *s);
/* 0: first edge armed; 1: interval produced; -1: invalid arguments; -2: nonmonotonic timestamp */
int cr_edge(cr_state_t *s, uint64_t timestamp_us, uint64_t *interval_us);
int32_t cr_error_ppm(uint64_t interval_us, uint64_t nominal_us);
#endif
