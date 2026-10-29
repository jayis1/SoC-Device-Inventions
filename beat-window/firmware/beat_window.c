#include "beat_window.h"

bool beat_window_crossing(struct beat_window *state, uint64_t now_us, uint64_t *interval_us) {
    if (!state || !interval_us) return false;
    if (!state->seen) {
        state->last_us = now_us;
        state->seen = true;
        return false;
    }
    *interval_us = now_us - state->last_us; /* unsigned subtraction tolerates timer wrap */
    state->last_us = now_us;
    return true;
}
