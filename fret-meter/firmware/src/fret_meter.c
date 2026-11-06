#include "fret_meter.h"
#include <stddef.h>

void fm_init(fm_state *s, float low_mm, float high_mm) {
    if (!s) return;
    s->low_mm = low_mm; s->high_mm = high_mm; s->sum = 0; s->count = 0;
}

void fm_push(fm_state *s, uint16_t raw) {
    if (!s || raw > 4095 || s->count >= 16) return;
    s->sum += raw; s->count++;
}

bool fm_reading(const fm_state *s, uint16_t *raw, float *mm) {
    if (!s || !raw || !mm || !s->count || s->low_mm == s->high_mm) return false;
    *raw = (uint16_t)(s->sum / s->count);
    *mm = ((float)*raw / 4095.0f) * (s->high_mm - s->low_mm) + s->low_mm;
    return true;
}
