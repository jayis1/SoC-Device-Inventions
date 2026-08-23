#include <math.h>
#include <stddef.h>

#include "porometer.h"

static float saturation_vapor_pressure_kpa(float temp_c)
{
    return 0.6108f * expf((17.27f * temp_c) / (temp_c + 237.3f));
}

static float vapor_density_g_m3(float temp_c, float rh_pct)
{
    const float es_kpa = saturation_vapor_pressure_kpa(temp_c);
    const float ea_kpa = es_kpa * (rh_pct / 100.0f);
    return 2166.7f * ea_kpa / (temp_c + 273.15f);
}

static float fit_slope(const float *x, const float *y, size_t n, float *r2)
{
    float sx = 0.0f, sy = 0.0f, sxx = 0.0f, sxy = 0.0f;
    float mean_y = 0.0f;

    for (size_t i = 0; i < n; ++i) {
        sx += x[i];
        sy += y[i];
        sxx += x[i] * x[i];
        sxy += x[i] * y[i];
        mean_y += y[i];
    }

    mean_y /= (float)n;

    const float denom = (float)n * sxx - sx * sx;
    const float slope = (fabsf(denom) < 1e-6f) ? 0.0f : (((float)n * sxy - sx * sy) / denom);
    const float intercept = (sy - slope * sx) / (float)n;

    float ss_tot = 0.0f;
    float ss_res = 0.0f;
    for (size_t i = 0; i < n; ++i) {
        const float pred = slope * x[i] + intercept;
        const float dy = y[i] - mean_y;
        const float er = y[i] - pred;
        ss_tot += dy * dy;
        ss_res += er * er;
    }

    *r2 = (ss_tot < 1e-9f) ? 1.0f : (1.0f - ss_res / ss_tot);
    return slope;
}

const char *sc_condition_name(sc_condition_t condition)
{
    switch (condition) {
    case SC_CONDITION_HYDRATED:
        return "hydrated";
    case SC_CONDITION_WATCH:
        return "watch";
    case SC_CONDITION_STRESSED:
        return "stressed";
    case SC_CONDITION_SEVERE:
        return "severe";
    default:
        return "unknown";
    }
}

void sc_compute_result(const sc_capture_t *capture, sc_result_t *result)
{
    float t_axis[SC_MAX_SAMPLES];
    float rho_axis[SC_MAX_SAMPLES];

    for (size_t i = 0; i < capture->count; ++i) {
        t_axis[i] = capture->sample_dt_s * (float)i;
        rho_axis[i] = vapor_density_g_m3(capture->chamber_temp_c[i], capture->chamber_rh_pct[i]);
    }

    result->vapor_slope_g_m3_s = fit_slope(t_axis, rho_axis, capture->count, &result->fit_r2);

    const float volume_m3 = capture->chamber_volume_cm3 * 1.0e-6f;
    const float area_m2 = capture->leaf_area_cm2 * 1.0e-4f;
    result->transpiration_g_m2_s = result->vapor_slope_g_m3_s * volume_m3 / area_m2;

    const float leaf_es = saturation_vapor_pressure_kpa(capture->leaf_temp_c);
    const float ambient_ea = saturation_vapor_pressure_kpa(capture->ambient_temp_c) * (capture->ambient_rh_pct / 100.0f);
    result->vpd_kpa = leaf_es - ambient_ea;
    if (result->vpd_kpa < 0.05f) {
        result->vpd_kpa = 0.05f;
    }

    {
        const float molar_flux = result->transpiration_g_m2_s / 18.01528f;
        result->stomatal_conductance_mol_m2_s = molar_flux / result->vpd_kpa * 1000.0f;
    }

    result->leaf_air_delta_c = capture->leaf_temp_c - capture->ambient_temp_c;

    {
        float score = 0.0f;
        if (result->stomatal_conductance_mol_m2_s < 0.25f) {
            score += (0.25f - result->stomatal_conductance_mol_m2_s) * 180.0f;
        }
        if (result->leaf_air_delta_c > -0.5f) {
            score += (result->leaf_air_delta_c + 0.5f) * 18.0f;
        }
        if (capture->light_lux_proxy > 12000.0f && result->stomatal_conductance_mol_m2_s < 0.18f) {
            score += 12.0f;
        }
        if (result->fit_r2 < 0.985f) {
            score += (0.985f - result->fit_r2) * 1000.0f;
        }
        if (score < 0.0f) {
            score = 0.0f;
        }
        if (score > 100.0f) {
            score = 100.0f;
        }
        result->stress_score = score;
    }

    if (result->stress_score < 35.0f) {
        result->condition = SC_CONDITION_HYDRATED;
    } else if (result->stress_score < 55.0f) {
        result->condition = SC_CONDITION_WATCH;
    } else if (result->stress_score < 80.0f) {
        result->condition = SC_CONDITION_STRESSED;
    } else {
        result->condition = SC_CONDITION_SEVERE;
    }
}
