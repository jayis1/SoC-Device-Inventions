// Copyright (c) jayis1. Inherits repository license.
#ifndef KNOT_WITNESS_CORE_H
#define KNOT_WITNESS_CORE_H
#include <stdbool.h>
#include <stdint.h>
typedef struct {
    int64_t offset;
    double counts_per_newton;
    double peak_newton;
    bool calibrated;
} kw_state_t;
void kw_init(kw_state_t *s, double counts_per_newton);
bool kw_tare(kw_state_t *s, const int32_t *samples, unsigned count);
bool kw_update(kw_state_t *s, int32_t raw, double *force_newton);
#endif
