#include "flow.h"

float pp_compute_concentration_grains_m3(unsigned particle_count,
                                         float flow_lpm,
                                         float sample_seconds,
                                         float sampled_area_mm2,
                                         float inlet_efficiency)
{
    float sampled_volume_m3 = (flow_lpm / 1000.0f / 60.0f) * sample_seconds;
    if (sampled_volume_m3 <= 0.0f || sampled_area_mm2 <= 0.0f || inlet_efficiency <= 0.0f) {
        return 0.0f;
    }

    /* Scale by the ratio between total tape field and imaged patch. */
    const float tape_band_mm2 = 14.0f;
    float expansion = tape_band_mm2 / sampled_area_mm2;
    return ((float)particle_count * expansion) / (sampled_volume_m3 * inlet_efficiency);
}

float pp_compute_allergen_index(float grains_per_m3, float weighted_score)
{
    float raw = grains_per_m3 * (0.15f + 0.1f * weighted_score);
    if (raw > 100.0f) {
        raw = 100.0f;
    }
    return raw;
}
