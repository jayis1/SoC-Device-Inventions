#include <math.h>
#include <stddef.h>
#include <string.h>

#include "sensors.h"

static void fill_pattern(double *samples, size_t count, double base, double burst_amp,
                         size_t burst_period, size_t burst_width, double ramp)
{
    for (size_t i = 0; i < count; ++i) {
        double pulse = ((i % burst_period) < burst_width) ? burst_amp : 0.0;
        double shape = base + pulse + ramp * (double) i;
        double ripple = 0.04 * sin((double) i * 0.43) + 0.02 * cos((double) i * 0.19);
        samples[i] = shape + ripple;
        if (samples[i] < 0.0) {
            samples[i] = 0.0;
        }
    }
}

size_t ts_load_demo_captures(ts_capture_t *captures, size_t max_captures)
{
    if ((captures == NULL) || (max_captures < 5U)) {
        return 0U;
    }

    memset(captures, 0, sizeof(ts_capture_t) * 5U);

    strcpy(captures[0].asset_id, "BLDG-A-TRAP-07");
    captures[0].upstream_temp_c = 158.0;
    captures[0].downstream_temp_c = 93.0;
    captures[0].ambient_temp_c = 24.5;
    captures[0].clamp_force_n = 17.5;
    captures[0].battery_v = 3.92;
    fill_pattern(captures[0].ultrasonic, TS_ENVELOPE_POINTS, 0.10, 0.70, 18U, 5U, -0.0002);

    strcpy(captures[1].asset_id, "BLDG-A-TRAP-11");
    captures[1].upstream_temp_c = 154.0;
    captures[1].downstream_temp_c = 142.0;
    captures[1].ambient_temp_c = 24.0;
    captures[1].clamp_force_n = 18.1;
    captures[1].battery_v = 3.88;
    fill_pattern(captures[1].ultrasonic, TS_ENVELOPE_POINTS, 0.76, 0.12, 11U, 10U, 0.0001);

    strcpy(captures[2].asset_id, "BLDG-B-TRAP-03");
    captures[2].upstream_temp_c = 141.0;
    captures[2].downstream_temp_c = 38.0;
    captures[2].ambient_temp_c = 23.2;
    captures[2].clamp_force_n = 16.8;
    captures[2].battery_v = 3.95;
    fill_pattern(captures[2].ultrasonic, TS_ENVELOPE_POINTS, 0.03, 0.05, 31U, 2U, 0.0);

    strcpy(captures[3].asset_id, "BLDG-C-TRAP-19");
    captures[3].upstream_temp_c = 149.0;
    captures[3].downstream_temp_c = 119.0;
    captures[3].ambient_temp_c = 25.1;
    captures[3].clamp_force_n = 15.9;
    captures[3].battery_v = 3.90;
    fill_pattern(captures[3].ultrasonic, TS_ENVELOPE_POINTS, 0.34, 0.20, 26U, 10U, 0.0003);

    strcpy(captures[4].asset_id, "BLDG-D-VALVE-02");
    captures[4].upstream_temp_c = 88.0;
    captures[4].downstream_temp_c = 84.0;
    captures[4].ambient_temp_c = 22.7;
    captures[4].clamp_force_n = 13.4;
    captures[4].battery_v = 3.86;
    fill_pattern(captures[4].ultrasonic, TS_ENVELOPE_POINTS, 0.51, 0.48, 7U, 2U, -0.0001);

    return 5U;
}
