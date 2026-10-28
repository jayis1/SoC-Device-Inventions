#include <stdint.h>
#include <stdbool.h>
#include "tape_tally.h"

static uint32_t last_edge;
static uint32_t last_reported_stall;
static bool initialized;

bool tt_sample(bool level, uint32_t now_ms, tt_event *event) {
    if (event == 0) return false;
    event->type = TT_NONE;
    event->value_ms = 0;
    if (level) {
        if (!initialized || (uint32_t)(now_ms - last_edge) >= 20u) {
            last_edge = now_ms;
            initialized = true;
            event->type = TT_PULSE;
            event->value_ms = now_ms;
            last_reported_stall = 0;
            return true;
        }
    }
    if (initialized) {
        uint32_t gap = (uint32_t)(now_ms - last_edge);
        if (gap >= 500u && last_reported_stall != last_edge) {
            last_reported_stall = last_edge;
            event->type = TT_STALL;
            event->value_ms = gap;
            return true;
        }
    }
    return false;
}
