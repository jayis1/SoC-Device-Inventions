#ifndef CHLORO_CLIP_H
#define CHLORO_CLIP_H

#include <stddef.h>

#define CC_CURVE_SAMPLES 12U
#define CC_LEAF_ID_LEN 24U

typedef enum {
    CC_DIAG_HEALTHY = 0,
    CC_DIAG_MILD_DROUGHT_STRESS,
    CC_DIAG_HEAT_STRESS,
    CC_DIAG_PHOTOINHIBITION,
    CC_DIAG_NITROGEN_DEFICIENCY,
    CC_DIAG_CLAMP_ERROR
} cc_diagnosis_t;

typedef struct {
    char leaf_id[CC_LEAF_ID_LEN];
    float leaf_temp_c;
    float ambient_temp_c;
    float ambient_par_umol;
    float battery_v;
    float dark_curve[CC_CURVE_SAMPLES];
    float light_curve[CC_CURVE_SAMPLES];
} cc_capture_t;

typedef struct {
    float fo;
    float fm;
    float fv_fm;
    float fs;
    float fm_prime;
    float fo_prime;
    float phi_psii;
    float npq;
    float qp;
    float stress_index;
    cc_diagnosis_t diagnosis;
} cc_result_t;

const char *cc_diagnosis_str(cc_diagnosis_t diagnosis);

#endif
