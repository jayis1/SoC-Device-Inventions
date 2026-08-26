#include "kinetics.h"

#include <math.h>
#include <string.h>

static float curve_max(const float *values)
{
    float max_value = values[0];
    for (size_t i = 1; i < CC_CURVE_SAMPLES; ++i) {
        if (values[i] > max_value) {
            max_value = values[i];
        }
    }
    return max_value;
}

static float tail_min3(const float *values)
{
    float min_value = values[CC_CURVE_SAMPLES - 3U];
    for (size_t i = CC_CURVE_SAMPLES - 2U; i < CC_CURVE_SAMPLES; ++i) {
        if (values[i] < min_value) {
            min_value = values[i];
        }
    }
    return min_value;
}

static float clampf(float value, float min_value, float max_value)
{
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

void cc_compute_metrics(const cc_capture_t *capture, cc_result_t *result)
{
    float ideal_fvfm = 0.83f;
    float ideal_phi = 0.55f;
    float fo;
    float fm;
    float fs;
    float fm_prime;
    float fo_prime;
    float fv_fm;
    float phi_psii;
    float npq;
    float qp;
    float stress;

    memset(result, 0, sizeof(*result));

    fo = capture->dark_curve[0];
    fm = curve_max(capture->dark_curve);
    fs = capture->light_curve[0];
    fm_prime = curve_max(capture->light_curve);
    fo_prime = tail_min3(capture->light_curve);

    fv_fm = (fm > 0.0f) ? ((fm - fo) / fm) : 0.0f;
    phi_psii = (fm_prime > 0.0f) ? ((fm_prime - fs) / fm_prime) : 0.0f;
    npq = (fm_prime > 0.0f) ? ((fm - fm_prime) / fm_prime) : 0.0f;
    qp = ((fm_prime - fo_prime) > 0.0f) ? ((fm_prime - fs) / (fm_prime - fo_prime)) : 0.0f;

    stress = 100.0f * (0.45f * clampf((ideal_fvfm - fv_fm) / 0.35f, 0.0f, 1.0f)
        + 0.35f * clampf((ideal_phi - phi_psii) / 0.55f, 0.0f, 1.0f)
        + 0.20f * clampf(npq / 2.0f, 0.0f, 1.0f));

    result->fo = fo;
    result->fm = fm;
    result->fv_fm = fv_fm;
    result->fs = fs;
    result->fm_prime = fm_prime;
    result->fo_prime = fo_prime;
    result->phi_psii = phi_psii;
    result->npq = npq;
    result->qp = qp;
    result->stress_index = stress;
}
