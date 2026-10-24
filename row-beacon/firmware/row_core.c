#include "row_core.h"
#include <limits.h>
void row_core_init(row_core_t *core, uint32_t debounce_ms) {
    if (!core) return;
    core->count = 0u;
    core->debounce_ms = debounce_ms;
    core->last_event_ms = 0u;
    core->have_event = false;
    core->last_level = true;
}
bool row_core_sample(row_core_t *core, bool hall_level, uint32_t now_ms) {
    if (!core) return false;
    bool falling = core->last_level && !hall_level;
    core->last_level = hall_level;
    if (!falling || (core->have_event && (uint32_t)(now_ms - core->last_event_ms) < core->debounce_ms)) return false;
    if (core->count == UINT32_MAX) return false;
    core->count++;
    core->last_event_ms = now_ms;
    core->have_event = true;
    return true;
}
bool row_core_undo(row_core_t *core) {
    if (!core || core->count == 0u) return false;
    core->count--;
    return true;
}
