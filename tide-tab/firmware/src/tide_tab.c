#include <stdbool.h>
#include <stdint.h>

#define DEBOUNCE_MS 50u

typedef void (*event_fn)(uint32_t ms, bool low, void *ctx);

/* Call at a steady cadence; raw_low is true when the switch closes to GND. */
void tide_tab_step(bool raw_low, uint32_t now_ms, bool *stable_low,
                   bool *initialized, bool *candidate, uint32_t *changed_ms,
                   event_fn emit, void *ctx) {
    if (!*initialized) {
        *stable_low = raw_low;
        *candidate = raw_low;
        *changed_ms = now_ms;
        *initialized = true;
        emit(now_ms, raw_low, ctx);
        return;
    }
    if (raw_low != *candidate) {
        *candidate = raw_low;
        *changed_ms = now_ms;
    }
    if (*candidate != *stable_low && (uint32_t)(now_ms - *changed_ms) >= DEBOUNCE_MS) {
        *stable_low = *candidate;
        emit(now_ms, *stable_low, ctx);
    }
}
