#ifndef STATIC_SENTINEL_ESD_CORE_H
#define STATIC_SENTINEL_ESD_CORE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    ESD_OK = 0,
    ESD_WARN_HUMIDITY,
    ESD_FAIL_MAT,
    ESD_FAIL_STRAP,
    ESD_INVALID_SAMPLE
} esd_status_t;

typedef struct {
    float mat_ohms;
    float strap_ohms;
    float relative_humidity_pct;
    bool interlock_closed;
} esd_sample_t;

typedef struct {
    float mat_min_ohms;
    float mat_max_ohms;
    float strap_min_ohms;
    float strap_max_ohms;
    float humidity_min_pct;
} esd_policy_t;

bool esd_ohms_from_divider(float v_supply, float r_reference_ohms, float v_node, float *ohms);
esd_status_t esd_evaluate(const esd_sample_t *sample, const esd_policy_t *policy);
const char *esd_status_name(esd_status_t status);

#endif
