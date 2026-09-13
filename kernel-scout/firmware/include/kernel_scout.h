#ifndef KERNEL_SCOUT_H
#define KERNEL_SCOUT_H

#include <stddef.h>

typedef enum { KS_DRY, KS_CONDITION, KS_AERATE, KS_URGENT_DRY, KS_INVALID } ks_storage_t;
typedef struct { const char *name; float a0, a1, a2, a3, baseline_mag, min_mass_g, max_mass_g; } ks_profile_t;
typedef struct { float magnitude[5], mass_g, grain_temp_c, ambient_temp_c, ambient_rh, impulses; } ks_sample_t;
typedef struct { float moisture_wb_pct, density_kg_m3, test_weight_kg_hl, insect_impulses_min, confidence_pct; int safe_days; ks_storage_t storage; } ks_result_t;
float ks_median5(const float values[5]);
void ks_analyze(const ks_profile_t *profile, const ks_sample_t *sample, ks_result_t *result);
const char *ks_storage_name(ks_storage_t value);
size_t ks_demo_samples(ks_sample_t *samples, size_t capacity);
#endif
