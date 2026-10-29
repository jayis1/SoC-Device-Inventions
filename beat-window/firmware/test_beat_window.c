#include "beat_window.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
int main(void) {
    struct beat_window s = {0}; uint64_t dt = 0;
    assert(!beat_window_crossing(&s, 100, &dt));
    assert(beat_window_crossing(&s, 1000100, &dt) && dt == 1000000);
    assert(beat_window_crossing(&s, 2000100, &dt) && dt == 1000000);
    s.last_us = UINT64_MAX - 4;
    assert(beat_window_crossing(&s, 5, &dt) && dt == 10);
    assert(!beat_window_crossing(NULL, 3, &dt));
    assert(!beat_window_crossing(&s, 3, NULL));
    return 0;
}
