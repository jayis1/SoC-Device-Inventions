#include "rain_core.h"
bool rain_accept_tip(rain_debounce_t *state, uint32_t now_ms)
{
    if (state == 0) return false;
    if (state->seen && (uint32_t)(now_ms - state->last_ms) < 40U) return false;
    state->last_ms = now_ms;
    state->seen = true;
    return true;
}
