// Copyright (c) jayis1. Inherits repository license.
#include "core.h"
void kw_init(kw_state_t *s, double scale) {
    s->offset = 0; s->counts_per_newton = scale; s->peak_newton = 0.0;
    s->calibrated = scale > 0.0;
}
bool kw_tare(kw_state_t *s, const int32_t *samples, unsigned count) {
    if (!s || !samples || count == 0 || count > 4096) return false;
    int64_t sum = 0;
    for (unsigned i=0; i<count; ++i) sum += samples[i];
    s->offset = sum / (int64_t)count;
    s->peak_newton = 0.0;
    return true;
}
bool kw_update(kw_state_t *s, int32_t raw, double *force) {
    if (!s || !force || !s->calibrated) return false;
    double f = (double)((int64_t)raw - s->offset) / s->counts_per_newton;
    if (f < 0.0) f = 0.0;
    if (f > s->peak_newton) s->peak_newton = f;
    *force = f;
    return true;
}
