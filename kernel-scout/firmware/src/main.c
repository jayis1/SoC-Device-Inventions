#include <stdio.h>
#include "kernel_scout.h"
int main(void) {
  const ks_profile_t wheat = {"wheat", 8.30f, 42.0f, -4.2f, -0.015f, 1000.0f, 80.0f, 108.0f};
  ks_sample_t samples[3]; ks_result_t out; size_t count = ks_demo_samples(samples, 3);
  puts("Kernel Scout reference firmware demo");
  for (size_t i = 0; i < count; ++i) { ks_analyze(&wheat, &samples[i], &out); printf("profile=%s moisture=%.2f%% density=%.1fkg/m3 test_weight=%.1fkg/hL insect=%.1f/min storage=%s safe_days=%d confidence=%.0f%%\n", wheat.name, out.moisture_wb_pct, out.density_kg_m3, out.test_weight_kg_hl, out.insect_impulses_min, ks_storage_name(out.storage), out.safe_days, out.confidence_pct); }
  return 0;
}
