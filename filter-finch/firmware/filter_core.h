#ifndef FILTER_CORE_H
#define FILTER_CORE_H

#include <stdbool.h>
#include <stddef.h>

#define FF_MAX_CALIBRATION_SAMPLES 64U

typedef enum {
    FF_OK = 0,
    FF_WARN_LOADING,
    FF_REPLACE,
    FF_FAN_OFF,
    FF_LOW_FLOW,
    FF_LEARN_REQUIRED,
    FF_TUBE_FAULT,
    FF_SENSOR_FAULT
} ff_status_t;

typedef struct {
    float filter_dp_pa;
    float flow_dp_pa;
    float temperature_c;
    float humidity_pct;
    bool filter_sensor_ok;
    bool flow_sensor_ok;
} ff_sample_t;

typedef struct {
    float reference_flow_dp_pa;
    float clean_normalized_dp_pa;
    float fan_off_dp_pa;
    float minimum_valid_flow_dp_pa;
    float warn_ratio;
    float replace_ratio;
} ff_config_t;

typedef struct {
    ff_status_t status;
    float normalized_filter_dp_pa;
    float loading_ratio;
} ff_result_t;

typedef struct {
    ff_status_t stable_status;
    ff_status_t pending_status;
    unsigned pending_count;
    unsigned required_count;
} ff_tracker_t;

ff_result_t ff_evaluate(const ff_sample_t *sample, const ff_config_t *config);
bool ff_calibrate_clean(const ff_sample_t *samples, size_t count,
                        const ff_config_t *config, float *baseline_pa);
void ff_tracker_init(ff_tracker_t *tracker, unsigned required_count);
ff_status_t ff_tracker_update(ff_tracker_t *tracker, ff_status_t candidate);
const char *ff_status_name(ff_status_t status);

#endif
