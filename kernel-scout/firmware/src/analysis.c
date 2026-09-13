#include <math.h>
#include "kernel_scout.h"

float ks_median5(const float values[5]) {
    float v[5];
    for (int i = 0; i < 5; ++i) v[i] = values[i];
    for (int i = 1; i < 5; ++i) { float key = v[i]; int j = i - 1; while (j >= 0 && v[j] > key) { v[j + 1] = v[j]; --j; } v[j + 1] = key; }
    return v[2];
}
const char *ks_storage_name(ks_storage_t v) {
    static const char *names[] = {"DRY", "CONDITION", "AERATE", "URGENT DRY", "INVALID"};
    return (v >= KS_DRY && v <= KS_INVALID) ? names[v] : "INVALID";
}
void ks_analyze(const ks_profile_t *p, const ks_sample_t *s, ks_result_t *r) {
    const float mag = ks_median5(s->magnitude);
    const float response = (mag - p->baseline_mag) / p->baseline_mag;
    const float spread = fabsf(s->magnitude[4] - s->magnitude[0]);
    float penalty;
    r->moisture_wb_pct = p->a0 + p->a1 * response + p->a2 * response * response + p->a3 * s->grain_temp_c;
    r->density_kg_m3 = s->mass_g / 0.120f;
    r->test_weight_kg_hl = s->mass_g / 1.2f;
    r->insect_impulses_min = s->impulses * 3.0f;
    if (s->mass_g < p->min_mass_g || s->mass_g > p->max_mass_g || spread > 0.012f * mag) { r->storage = KS_INVALID; r->safe_days = 0; r->confidence_pct = 0; return; }
    penalty = fmaxf(0.0f, r->moisture_wb_pct - 12.5f) * 0.45f + fmaxf(0.0f, s->grain_temp_c - 15.0f) * 0.025f + r->insect_impulses_min * 0.035f + fmaxf(0.0f, s->ambient_rh - 65.0f) * 0.01f;
    r->safe_days = (int)fmaxf(1.0f, fminf(180.0f, 180.0f * expf(-penalty)));
    r->storage = r->moisture_wb_pct >= 17.0f || r->insect_impulses_min >= 30.0f ? KS_URGENT_DRY : r->moisture_wb_pct >= 14.5f || s->grain_temp_c >= 28.0f ? KS_AERATE : r->moisture_wb_pct >= 13.0f ? KS_CONDITION : KS_DRY;
    r->confidence_pct = fmaxf(55.0f, fminf(99.0f, 98.0f - spread / mag * 1000.0f - fabsf(s->mass_g - (p->min_mass_g + p->max_mass_g) * .5f) * .08f));
}
