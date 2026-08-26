#include "classifier.h"

#include "kinetics.h"

const char *cc_diagnosis_str(cc_diagnosis_t diagnosis)
{
    switch (diagnosis) {
    case CC_DIAG_HEALTHY:
        return "HEALTHY";
    case CC_DIAG_MILD_DROUGHT_STRESS:
        return "MILD_DROUGHT_STRESS";
    case CC_DIAG_HEAT_STRESS:
        return "HEAT_STRESS";
    case CC_DIAG_PHOTOINHIBITION:
        return "PHOTOINHIBITION";
    case CC_DIAG_NITROGEN_DEFICIENCY:
        return "NITROGEN_DEFICIENCY";
    case CC_DIAG_CLAMP_ERROR:
    default:
        return "CLAMP_ERROR";
    }
}

void cc_classify_capture(const cc_capture_t *capture, cc_result_t *result)
{
    cc_compute_metrics(capture, result);

    if ((result->fm <= (result->fo + 20.0f)) || (result->fm_prime <= (result->fs + 15.0f))) {
        result->diagnosis = CC_DIAG_CLAMP_ERROR;
    } else if (result->fv_fm < 0.63f) {
        result->diagnosis = CC_DIAG_PHOTOINHIBITION;
    } else if ((capture->leaf_temp_c >= 38.0f) && (result->fv_fm < 0.72f) && (result->phi_psii < 0.25f)) {
        result->diagnosis = CC_DIAG_HEAT_STRESS;
    } else if ((capture->leaf_temp_c >= 33.0f) && (result->fv_fm < 0.81f) && (result->phi_psii < 0.30f) && (result->qp < 0.45f)) {
        result->diagnosis = CC_DIAG_MILD_DROUGHT_STRESS;
    } else if ((result->fv_fm >= 0.78f) && (result->phi_psii < 0.24f) && (result->qp < 0.40f)) {
        result->diagnosis = CC_DIAG_NITROGEN_DEFICIENCY;
    } else {
        result->diagnosis = CC_DIAG_HEALTHY;
    }
}
