#include "sensors.h"

void sc_make_demo_capture(sc_capture_t *capture)
{
    static const float temp_trace[SC_MAX_SAMPLES] = {
        29.80f, 29.83f, 29.86f, 29.89f, 29.93f, 29.96f,
        29.98f, 30.01f, 30.04f, 30.07f, 30.10f, 30.12f,
        30.15f, 30.17f, 30.19f, 30.21f, 30.24f, 30.26f
    };

    static const float rh_trace[SC_MAX_SAMPLES] = {
        44.8f, 45.9f, 47.0f, 48.1f, 49.3f, 50.4f,
        51.7f, 52.9f, 54.1f, 55.4f, 56.6f, 57.9f,
        59.0f, 60.2f, 61.4f, 62.5f, 63.7f, 64.9f
    };

    capture->ambient_temp_c = 31.10f;
    capture->ambient_rh_pct = 43.50f;
    capture->leaf_temp_c = 29.42f;
    capture->light_lux_proxy = 18650.0f;
    capture->battery_v = 3.91f;
    capture->blower_current_a = 0.118f;
    capture->chamber_volume_cm3 = 7.6f;
    capture->leaf_area_cm2 = 4.8f;
    capture->sample_dt_s = 1.0f;
    capture->count = SC_MAX_SAMPLES;

    for (size_t i = 0; i < SC_MAX_SAMPLES; ++i) {
        capture->chamber_temp_c[i] = temp_trace[i];
        capture->chamber_rh_pct[i] = rh_trace[i];
    }
}
