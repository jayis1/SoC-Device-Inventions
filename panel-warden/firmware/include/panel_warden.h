#ifndef PANEL_WARDEN_H
#define PANEL_WARDEN_H

#include <stdbool.h>
#include <stddef.h>

#define PW_MAX_NOTES 96U

typedef enum {
    PW_NORMAL = 0,
    PW_OVERLOAD_HEATING,
    PW_LOOSE_LUG,
    PW_CORONA_PD,
    PW_SURFACE_TRACKING,
    PW_INTERMITTENT_ARC,
    PW_CONDENSATION_RISK
} pw_fault_t;

typedef struct {
    const char *asset_id;
    float hotspot_c;
    float background_c;
    float hotspot_area_px;
    float rise_rate_c_per_hr;
    float ultrasound_db;
    float rf_dbm_equiv;
    float burst_rate_hz;
    float load_corr_a;
    float humidity_pct;
    float ambient_c;
    float contact_c;
    bool ct_present;
} pw_frame_t;

typedef struct {
    pw_fault_t fault;
    float delta_t_c;
    float dewpoint_c;
    float dew_margin_c;
    float risk_score;
    float confidence;
    char note[PW_MAX_NOTES];
} pw_result_t;

const char *pw_fault_name(pw_fault_t fault);
float pw_compute_dewpoint_c(float ambient_c, float humidity_pct);
float pw_clampf(float value, float min_value, float max_value);

#endif
