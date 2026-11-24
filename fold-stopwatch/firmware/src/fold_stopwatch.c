#include "fold_stopwatch.h"

void fs_init(fs_state_t *state)
{
    if (state == 0) return;
    state->last_gate = -1;
    state->last_time_us = 0;
    state->sequence = 0;
    state->started = false;
}

bool fs_edge(fs_state_t *state, uint8_t gate, uint64_t time_us, fs_event_t *event)
{
    if (state == 0 || event == 0 || gate > 1) return false;
    if (state->started && (gate == (uint8_t)state->last_gate || time_us < state->last_time_us)) return false;
    uint64_t elapsed = state->started ? time_us - state->last_time_us : 0;
    if (state->started && elapsed > FS_MAX_GAP_US) {
        state->started = false;
        state->last_gate = -1;
        return false;
    }
    state->started = true;
    state->last_gate = (int)gate;
    state->last_time_us = time_us;
    event->sequence = ++state->sequence;
    event->gate = gate;
    event->elapsed_us = elapsed;
    return true;
}
