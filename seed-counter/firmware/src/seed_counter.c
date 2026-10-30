#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t count;
    bool blocked;
    uint32_t last_edge_ms;
} seed_counter_t;

/* Call with debounced beam state; one count per clear-to-block transition. */
bool seed_counter_update(seed_counter_t *s, bool blocked, uint32_t now_ms,
                         uint32_t debounce_ms) {
    if (blocked == s->blocked) return false;
    if ((uint32_t)(now_ms - s->last_edge_ms) < debounce_ms) return false;
    s->blocked = blocked;
    s->last_edge_ms = now_ms;
    if (blocked) { ++s->count; return true; }
    return false;
}

void seed_counter_reset(seed_counter_t *s) {
    s->count = 0;
    s->blocked = false;
    s->last_edge_ms = 0;
}
