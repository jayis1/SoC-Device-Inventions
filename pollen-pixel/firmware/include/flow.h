#ifndef POLLEN_PIXEL_FLOW_H
#define POLLEN_PIXEL_FLOW_H

float pp_compute_concentration_grains_m3(unsigned particle_count,
                                         float flow_lpm,
                                         float sample_seconds,
                                         float sampled_area_mm2,
                                         float inlet_efficiency);

float pp_compute_allergen_index(float grains_per_m3, float weighted_score);

#endif
