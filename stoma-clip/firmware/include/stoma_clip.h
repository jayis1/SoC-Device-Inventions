#ifndef STOMA_CLIP_H
#define STOMA_CLIP_H

#include <stddef.h>

#define SC_MAX_SAMPLES 18

typedef struct {
    float chamber_temp_c[SC_MAX_SAMPLES];
    float chamber_rh_pct[SC_MAX_SAMPLES];
    float ambient_temp_c;
    float ambient_rh_pct;
    float leaf_temp_c;
    float light_lux_proxy;
    float battery_v;
    float blower_current_a;
    float chamber_volume_cm3;
    float leaf_area_cm2;
    float sample_dt_s;
    size_t count;
} sc_capture_t;

typedef enum {
    SC_CONDITION_HYDRATED = 0,
    SC_CONDITION_WATCH,
    SC_CONDITION_STRESSED,
    SC_CONDITION_SEVERE
} sc_condition_t;

typedef struct {
    float vpd_kpa;
    float vapor_slope_g_m3_s;
    float transpiration_g_m2_s;
    float stomatal_conductance_mol_m2_s;
    float stress_score;
    float fit_r2;
    float leaf_air_delta_c;
    sc_condition_t condition;
} sc_result_t;

const char *sc_condition_name(sc_condition_t condition);

#endif
