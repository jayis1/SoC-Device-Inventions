#ifndef LINE_SHEPHERD_H
#define LINE_SHEPHERD_H

#include <stddef.h>

#define LS_MAX_NAME 32

typedef enum {
    LS_OPEN_END = 0,
    LS_SHORT_END,
    LS_HIGH_RESISTANCE_SPLICE,
    LS_WATER_INGRESS,
    LS_BRIDGE_TAP,
    LS_SPLIT_PAIR,
    LS_HEALTHY_LINE,
    LS_OVERVOLTAGE_LOCKOUT,
    LS_UNKNOWN_FAULT
} ls_fault_t;

typedef struct {
    char asset_id[LS_MAX_NAME];
    char pair_name[12];
    char cable_type[20];
    float velocity_factor;
    float round_trip_ns;
    float secondary_ns;
    float refl_peak_v;
    float refl_trough_v;
    float ringing_ratio;
    float loop_res_ohm;
    float cap_nf;
    float pair_balance_pct;
    float foreign_voltage_v;
    int remote_id;
} ls_capture_t;

typedef struct {
    ls_fault_t fault;
    float est_distance_m;
    float health_score;
    float confidence;
    float impedance_hint_ohm;
    const char *notes;
} ls_result_t;

const char *ls_fault_name(ls_fault_t fault);

#endif
