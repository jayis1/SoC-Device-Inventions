#include "measurement.h"
#include "config.h"
#include <math.h>
void pitch_init(pitch_stats_t *s) {
    if (!s) return;
    *s = (pitch_stats_t){ .min_mm = PATCH_CARD_MAX_INTERVAL_MM };
}
bool pitch_mark(pitch_stats_t *s, float travel_mm) {
    if (!s || !isfinite(travel_mm)) return false;
    if (!s->have_last) { s->last_mm = travel_mm; s->have_last = true; return false; }
    float d = travel_mm - s->last_mm;
    s->last_mm = travel_mm;
    if (d < PATCH_CARD_MIN_INTERVAL_MM || d > PATCH_CARD_MAX_INTERVAL_MM) { s->rejected++; return false; }
    s->sum_mm += d;
    if (d < s->min_mm) s->min_mm = d;
    if (d > s->max_mm) s->max_mm = d;
    s->accepted++;
    return true;
}
float pitch_mean(const pitch_stats_t *s) { return (!s || !s->accepted) ? 0.0f : s->sum_mm / (float)s->accepted; }
