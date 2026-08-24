#ifndef DRAFT_RANGER_H
#define DRAFT_RANGER_H

#include <stddef.h>

#define DR_AUDIO_SAMPLES 160

typedef struct {
    double pressure_pa;
    double bridge_v;
    double ambient_temp_c;
    double ambient_rh_pct;
    double frame_temp_c;
    double surface_temp_c;
    double battery_v;
    int mark_index;
    double audio[DR_AUDIO_SAMPLES];
} dr_capture_t;

typedef enum {
    DR_SEAL_OK = 0,
    DR_MINOR_WEATHERSTRIP_GAP,
    DR_PRESSURE_DRIVEN_DRAFT,
    DR_WARPED_FRAME_OR_SASH,
    DR_CONDENSATION_RISK,
    DR_CRITICAL_COMBINED
} dr_fault_class_t;

typedef struct {
    double dew_point_c;
    double condensation_margin_c;
    double draft_velocity_mps;
    double whistle_hz;
    double whistle_score;
    double leak_severity;
    double seal_health;
    dr_fault_class_t fault_class;
} dr_result_t;

const char *dr_fault_class_name(dr_fault_class_t fault_class);

#endif
