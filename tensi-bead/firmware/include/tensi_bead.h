#ifndef TENSI_BEAD_H
#define TENSI_BEAD_H

#include <stddef.h>
#include <stdbool.h>

typedef enum {
    TB_MODE_PENDANT = 0,
    TB_MODE_SESSILE = 1
} tb_mode_t;

typedef struct {
    unsigned sample_id;
    tb_mode_t mode;
    float pixel_scale_mm;
    float neck_px;
    float equator_px;
    float height_px;
    float base_width_px;
    float left_contact_deg;
    float right_contact_deg;
    float fluid_density_g_ml;
    float ambient_temp_c;
    float ambient_rh_pct;
    float dose_mass_mg;
    float illumination_lux;
} tb_frame_t;

typedef struct {
    float surface_tension_mn_m;
    float contact_angle_deg;
    float bond_number;
    float spread_index;
    const char *classification;
    bool qc_ok;
} tb_result_t;

const char *tb_mode_name(tb_mode_t mode);

#endif
