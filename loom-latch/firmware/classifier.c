#include "classifier.h"
#define MIN_US 300u
#define MAX_US 500000u
int classifier_edge(classifier_t *c, bool is_a, uint32_t now_us, uint32_t *duration_us, bool *a_to_b) {
    if (!c || !duration_us || !a_to_b) return 0;
    if (!c->armed) { c->armed = true; c->first_is_a = is_a; c->started_us = now_us; return 0; }
    uint32_t dt = now_us - c->started_us; /* unsigned wrap-safe */
    bool ordered = is_a != c->first_is_a;
    c->armed = false;
    if (!ordered || dt < MIN_US || dt > MAX_US) return 0;
    *duration_us = dt;
    *a_to_b = c->first_is_a;
    return 1;
}
