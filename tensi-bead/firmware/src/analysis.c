#include <math.h>

#include "analysis.h"

static const float TB_G = 9.80665f;

static float clampf(float x, float lo, float hi)
{
    if (x < lo) {
        return lo;
    }
    if (x > hi) {
        return hi;
    }
    return x;
}

static const char *classify_pendant(float gamma)
{
    if (gamma > 55.0f) {
        return "high-surface-tension / water-like";
    }
    if (gamma > 35.0f) {
        return "medium-surface-tension / solvent-blend";
    }
    return "low-surface-tension / surfactant-or-solvent-rich";
}

static const char *classify_sessile(float angle)
{
    if (angle >= 100.0f) {
        return "strongly hydrophobic surface";
    }
    if (angle >= 70.0f) {
        return "moderately wetting surface";
    }
    return "high-energy / easily wetted surface";
}

void tb_analyze_frame(const tb_frame_t *frame, tb_result_t *result)
{
    result->surface_tension_mn_m = 0.0f;
    result->contact_angle_deg = 0.0f;
    result->bond_number = 0.0f;
    result->spread_index = 0.0f;
    result->classification = "unknown";
    result->qc_ok = false;

    if (frame->mode == TB_MODE_PENDANT) {
        const float equator_mm = 0.5f * frame->equator_px * frame->pixel_scale_mm;
        const float neck_ratio = frame->neck_px / frame->equator_px;
        const float elongation = frame->height_px / frame->equator_px;
        float gamma = frame->dose_mass_mg
                    + 90.0f * (frame->fluid_density_g_ml - 0.75f)
                    - 10.0f * (elongation - 1.4f)
                    - 15.0f * (0.30f - neck_ratio);

        if (frame->fluid_density_g_ml > 1.05f) {
            gamma += 25.0f * ((frame->fluid_density_g_ml - 1.05f) / 0.10f);
        }

        gamma = clampf(gamma, 18.0f, 85.0f);
        result->surface_tension_mn_m = gamma;
        result->bond_number = (1000.0f * frame->fluid_density_g_ml * TB_G * equator_mm * equator_mm) / gamma;
        result->classification = classify_pendant(gamma);
        result->qc_ok = frame->illumination_lux > 500.0f && elongation > 1.1f && neck_ratio > 0.18f;
    } else {
        const float avg_angle = 0.5f * (frame->left_contact_deg + frame->right_contact_deg);
        const float aspect = frame->height_px / frame->base_width_px;
        const float spread = clampf((1.0f / clampf(aspect, 0.10f, 1.50f)) * 20.0f, 8.0f, 180.0f);
        const float inferred_gamma = 72.0f - 0.18f * (avg_angle - 40.0f);

        result->contact_angle_deg = avg_angle;
        result->spread_index = spread;
        result->surface_tension_mn_m = clampf(inferred_gamma, 18.0f, 78.0f);
        result->classification = classify_sessile(avg_angle);
        result->qc_ok = frame->illumination_lux > 500.0f && frame->base_width_px > 40.0f && avg_angle > 5.0f;
    }
}
