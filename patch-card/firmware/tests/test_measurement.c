#include "measurement.h"
#include <assert.h>
#include <math.h>
int main(void) {
    pitch_stats_t s;
    pitch_init(&s);
    assert(!pitch_mark(&s, 0.0f));
    assert(pitch_mark(&s, 2.5f));
    assert(pitch_mark(&s, 5.0f));
    assert(fabsf(pitch_mean(&s) - 2.5f) < 0.001f);
    assert(!pitch_mark(&s, 100.0f));
    assert(s.accepted == 2 && s.rejected == 1);
    assert(!pitch_mark(&s, NAN));
    return 0;
}
