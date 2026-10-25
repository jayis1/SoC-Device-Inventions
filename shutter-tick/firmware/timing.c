#include "timing.h"
bool shutter_capture_edge(shutter_capture_t *s, bool light_present,
                          uint64_t now_us, uint32_t *duration_us) {
    if (!s || !duration_us) return false;
    if (s->waiting_for_rise) {
        if (light_present) { s->rise_us = now_us; s->waiting_for_rise = false; }
        return false;
    }
    if (!light_present) {
        uint64_t elapsed = now_us - s->rise_us;
        s->waiting_for_rise = true;
        if (elapsed == 0 || elapsed > 1000000u) return false;
        *duration_us = (uint32_t)elapsed;
        return true;
    }
    return false;
}
