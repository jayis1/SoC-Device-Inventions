#ifndef CHILL_WARDEN_H
#define CHILL_WARDEN_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CW_MODE_COOL = 0,
    CW_MODE_HEAT,
    CW_MODE_FREEZER,
    CW_MODE_IDLE
} cw_mode_t;

typedef enum {
    CW_FAULT_NORMAL = 0,
    CW_FAULT_AIRFLOW_RESTRICTED,
    CW_FAULT_LOW_CHARGE_SUSPECT,
    CW_FAULT_BEARING_WEAR,
    CW_FAULT_LIQUID_SLUGGING,
    CW_FAULT_SHORT_CYCLING,
    CW_FAULT_DEFROST_ISSUE
} cw_fault_t;

typedef struct {
    const char *asset_id;
    cw_mode_t mode;
    double suction_c;
    double discharge_c;
    double ambient_c;
    double rh_pct;
    double current_a;
    double vibration_g;
    double acoustic_db;
    double piezo_db;
    double cycle_period_s;
    double superheat_proxy_c;
    double condenser_split_c;
} cw_frame_t;

typedef struct {
    cw_fault_t fault;
    double risk_score;
    char reason[160];
    char action[160];
} cw_result_t;

const char *cw_mode_string(cw_mode_t mode);
const char *cw_fault_string(cw_fault_t fault);

#ifdef __cplusplus
}
#endif

#endif
