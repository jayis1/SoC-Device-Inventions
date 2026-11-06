#include "row_meter.h"
#include <limits.h>

void row_meter_init(row_meter_t *m, uint32_t edges, uint32_t circumference) {
    if (!m) return;
    m->ticks = 0; m->events = 0; m->previous_event_mm = 0;
    m->edges_per_revolution = edges; m->circumference_mm = circumference;
}
void row_meter_edge(row_meter_t *m) {
    if (m && m->ticks != UINT64_MAX) ++m->ticks;
}
uint64_t row_meter_distance_mm(const row_meter_t *m) {
    if (!m || !m->edges_per_revolution || !m->circumference_mm) return 0;
    /* Divide before multiply with remainder to avoid overflow. */
    uint64_t q = m->ticks / m->edges_per_revolution;
    uint64_t r = m->ticks % m->edges_per_revolution;
    if (q > UINT64_MAX / m->circumference_mm) return UINT64_MAX;
    uint64_t whole = q * m->circumference_mm;
    uint64_t part = (r * (uint64_t)m->circumference_mm) / m->edges_per_revolution;
    return UINT64_MAX - whole < part ? UINT64_MAX : whole + part;
}
bool row_meter_mark(row_meter_t *m, uint64_t *spacing) {
    if (!m || !spacing) return false;
    uint64_t now = row_meter_distance_mm(m);
    *spacing = now - m->previous_event_mm;
    m->previous_event_mm = now;
    if (m->events != UINT32_MAX) ++m->events;
    return true;
}
