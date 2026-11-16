#include "mate_trace.h"
#include <limits.h>

void mate_trace_init(mate_trace_t *s) {
    if (s) { s->run_id = 0; s->peak_counts = 0; s->sample_count = 0; s->active = false; }
}
bool mate_trace_start(mate_trace_t *s, uint32_t id) {
    if (!s || s->active || id == 0) return false;
    s->run_id = id; s->peak_counts = 0; s->sample_count = 0; s->active = true;
    return true;
}
bool mate_trace_sample(mate_trace_t *s, int32_t counts) {
    if (!s || !s->active || s->sample_count == UINT32_MAX) return false;
    if (counts == INT32_MIN) return false; /* sentinel for adapter-detected ADC fault */
    const int32_t magnitude = counts < 0 ? -counts : counts;
    if (magnitude > s->peak_counts) s->peak_counts = magnitude;
    s->sample_count++;
    return true;
}
bool mate_trace_finish(mate_trace_t *s) {
    if (!s || !s->active || s->sample_count == 0) return false;
    s->active = false;
    return true;
}
