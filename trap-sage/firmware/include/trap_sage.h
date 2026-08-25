#ifndef TRAP_SAGE_H
#define TRAP_SAGE_H

#include <stddef.h>

#define TS_ENVELOPE_POINTS 96

typedef struct {
    char asset_id[24];
    double upstream_temp_c;
    double downstream_temp_c;
    double ambient_temp_c;
    double clamp_force_n;
    double battery_v;
    double ultrasonic[TS_ENVELOPE_POINTS];
} ts_capture_t;

typedef enum {
    TS_NORMAL_CYCLING = 0,
    TS_BLOW_THROUGH,
    TS_COLD_BLOCKED,
    TS_FLOODING,
    TS_CAVITATING_VALVE,
    TS_SENSOR_MISPLACED
} ts_fault_class_t;

typedef struct {
    double upstream_superheat_c;
    double delta_t_c;
    double ultrasonic_rms;
    double crest_factor;
    double burstiness;
    double active_fraction;
    double cycle_rate_cpm;
    double health_score;
    ts_fault_class_t fault_class;
} ts_result_t;

const char *ts_fault_class_name(ts_fault_class_t fault_class);

#endif
