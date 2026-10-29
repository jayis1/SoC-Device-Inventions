// Copyright (c) jayis1. Inherits repository license.
#include "core.h"
#include <assert.h>
#include <math.h>
int main(void) {
    kw_state_t s; double f=0;
    kw_init(&s, 0.0);
    assert(!kw_update(&s, 1100, &f));
    kw_init(&s, 100.0);
    int32_t tare[] = {1000, 1001, 999};
    assert(kw_tare(&s, tare, 3));
    assert(s.offset == 1000);
    assert(kw_update(&s, 1500, &f) && fabs(f-5.0)<1e-9);
    assert(kw_update(&s, 900, &f) && f == 0.0);
    assert(fabs(s.peak_newton-5.0)<1e-9);
    assert(!kw_tare(&s, tare, 0));
    assert(!kw_tare(&s, tare, 4097));
    return 0;
}
