#include "edge_tally.h"

#define DEBOUNCE_MS 25u

void edge_tally_init(edge_tally_t *s, bool initial_high, uint32_t now_ms) {
    s->stable = initial_high;
    s->candidate = initial_high;
    s->since_ms = now_ms;
    s->count = 0;
}

void edge_tally_sample(edge_tally_t *s, bool high, uint32_t now_ms) {
    if (high != s->candidate) {
        s->candidate = high;
        s->since_ms = now_ms;
    }
    if (s->candidate != s->stable && (uint32_t)(now_ms - s->since_ms) >= DEBOUNCE_MS) {
        s->stable = s->candidate;
        if (!s->stable && s->count != UINT32_MAX) ++s->count;
    }
}
