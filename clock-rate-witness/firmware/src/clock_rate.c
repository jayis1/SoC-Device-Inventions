#include "clock_rate.h"

void cr_init(cr_state_t *s) {
    if (s) { s->last_us = 0; s->count = 0; s->armed = 0; }
}

int cr_edge(cr_state_t *s, uint64_t timestamp_us, uint64_t *interval_us) {
    if (!s || !interval_us) return -1;
    *interval_us = 0;
    if (!s->armed) { s->last_us = timestamp_us; s->armed = 1; return 0; }
    if (timestamp_us <= s->last_us) return -2;
    *interval_us = timestamp_us - s->last_us;
    s->last_us = timestamp_us;
    s->count++;
    return 1;
}

int32_t cr_error_ppm(uint64_t interval_us, uint64_t nominal_us) {
    if (!nominal_us) return 0;
    int64_t d = (int64_t)interval_us - (int64_t)nominal_us;
    return (int32_t)((d * 1000000LL) / (int64_t)nominal_us);
}
