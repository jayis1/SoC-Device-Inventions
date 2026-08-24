#include <math.h>
#include <string.h>

#include "sensors.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static void populate_audio(double *buffer, double base_amp, double tone_amp, double tone_hz)
{
    const double sample_rate = 16000.0;

    for (size_t i = 0; i < DR_AUDIO_SAMPLES; ++i) {
        double t = (double)i / sample_rate;
        double broadband = base_amp * (0.55 * sin(2.0 * M_PI * 700.0 * t)
                                     + 0.35 * sin(2.0 * M_PI * 1450.0 * t + 0.2)
                                     + 0.10 * sin(2.0 * M_PI * 3200.0 * t + 1.3));
        double tonal = tone_amp * sin(2.0 * M_PI * tone_hz * t);
        buffer[i] = broadband + tonal;
    }
}

size_t dr_load_demo_captures(dr_capture_t *captures, size_t max_captures)
{
    if (captures == NULL || max_captures < 4U) {
        return 0U;
    }

    memset(captures, 0, sizeof(dr_capture_t) * max_captures);

    captures[0].pressure_pa = 3.2;
    captures[0].bridge_v = 0.18;
    captures[0].ambient_temp_c = 22.1;
    captures[0].ambient_rh_pct = 43.0;
    captures[0].frame_temp_c = 19.3;
    captures[0].surface_temp_c = 19.8;
    captures[0].battery_v = 3.96;
    captures[0].mark_index = 0;
    populate_audio(captures[0].audio, 0.03, 0.00, 1500.0);

    captures[1].pressure_pa = 14.5;
    captures[1].bridge_v = 0.74;
    captures[1].ambient_temp_c = 21.8;
    captures[1].ambient_rh_pct = 47.5;
    captures[1].frame_temp_c = 15.1;
    captures[1].surface_temp_c = 14.2;
    captures[1].battery_v = 3.90;
    captures[1].mark_index = 1;
    populate_audio(captures[1].audio, 0.08, 0.06, 1850.0);

    captures[2].pressure_pa = 29.0;
    captures[2].bridge_v = 1.12;
    captures[2].ambient_temp_c = 20.9;
    captures[2].ambient_rh_pct = 51.0;
    captures[2].frame_temp_c = 11.3;
    captures[2].surface_temp_c = 10.4;
    captures[2].battery_v = 3.87;
    captures[2].mark_index = 2;
    populate_audio(captures[2].audio, 0.10, 0.15, 2600.0);

    captures[3].pressure_pa = 8.0;
    captures[3].bridge_v = 0.30;
    captures[3].ambient_temp_c = 22.6;
    captures[3].ambient_rh_pct = 58.0;
    captures[3].frame_temp_c = 11.1;
    captures[3].surface_temp_c = 10.8;
    captures[3].battery_v = 3.83;
    captures[3].mark_index = 3;
    populate_audio(captures[3].audio, 0.05, 0.01, 900.0);

    return 4U;
}
