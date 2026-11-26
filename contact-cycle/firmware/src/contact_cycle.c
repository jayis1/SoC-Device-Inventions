#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define CC_CAPACITY 256u

typedef struct { uint32_t ms; uint8_t closed; } cc_event;
typedef struct { cc_event events[CC_CAPACITY]; size_t count; uint8_t last; uint32_t last_ms; int overflow; } cc_capture;

void cc_init(cc_capture *c, uint8_t initial_closed, uint32_t now_ms) {
    if (!c) return;
    c->count = 0; c->last = (uint8_t)(initial_closed != 0); c->last_ms = now_ms; c->overflow = 0;
}

/* Call only when a debounced state is available. Timestamps must be monotonic modulo uint32_t. */
int cc_sample(cc_capture *c, uint8_t closed, uint32_t now_ms) {
    if (!c || now_ms < c->last_ms) return -1;
    closed = (uint8_t)(closed != 0);
    if (closed == c->last) return 0;
    if (c->count >= CC_CAPACITY) { c->overflow = 1; return -2; }
    c->events[c->count].ms = now_ms;
    c->events[c->count].closed = closed;
    c->count++; c->last = closed; c->last_ms = now_ms;
    return 1;
}

int cc_write_csv(FILE *out, const cc_capture *c) {
    size_t i;
    if (!out || !c) return -1;
    if (fprintf(out, "time_ms,closed\n") < 0) return -1;
    for (i = 0; i < c->count; ++i)
        if (fprintf(out, "%u,%u\n", (unsigned)c->events[i].ms,
                    (unsigned)c->events[i].closed) < 0) return -1;
    return ferror(out) ? -1 : 0;
}

#ifdef CONTACT_CYCLE_TEST
int main(void) {
    cc_capture c;
    cc_init(&c, 0, 0);
    if (cc_sample(&c, 1, 10) != 1 || cc_sample(&c, 1, 11) != 0 || cc_sample(&c, 0, 22) != 1) return 1;
    if (cc_sample(&c, 1, 21) != -1 || c.count != 2) return 2;
    if (cc_write_csv(stdout, &c) != 0) return 3;
    return 0;
}
#endif
