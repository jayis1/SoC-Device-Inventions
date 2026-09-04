#include <string.h>

#include "sensors.h"

static const rf_preset_t PRESET_AUTO_12V = {
    .name = "automotive_relay_12v",
    .nominal_voltage = 12.0f,
    .max_pull_in_v = 10.5f,
    .min_drop_out_v = 1.5f,
    .max_drop_out_v = 5.0f,
    .max_operate_ms = 12.0f,
    .max_bounce_ms = 3.0f,
    .min_travel_mm = 1.1f,
    .min_coil_resistance_ohm = 55.0f,
    .release_timeout_ms = 20.0f
};

static const rf_preset_t PRESET_PLC_24V = {
    .name = "plc_relay_24v",
    .nominal_voltage = 24.0f,
    .max_pull_in_v = 23.0f,
    .min_drop_out_v = 3.0f,
    .max_drop_out_v = 10.0f,
    .max_operate_ms = 14.0f,
    .max_bounce_ms = 4.0f,
    .min_travel_mm = 1.3f,
    .min_coil_resistance_ohm = 180.0f,
    .release_timeout_ms = 22.0f
};

static const rf_preset_t PRESET_CONTACTOR_AUX = {
    .name = "contactor_aux_24v",
    .nominal_voltage = 24.0f,
    .max_pull_in_v = 21.0f,
    .min_drop_out_v = 3.0f,
    .max_drop_out_v = 9.0f,
    .max_operate_ms = 15.0f,
    .max_bounce_ms = 5.0f,
    .min_travel_mm = 1.4f,
    .min_coil_resistance_ohm = 200.0f,
    .release_timeout_ms = 24.0f
};

static void fill_event(rf_event_t *event,
                       const char *timestamp,
                       rf_test_mode_t mode,
                       const rf_preset_t *preset,
                       const rf_sample_t *samples,
                       size_t sample_count)
{
    event->timestamp = timestamp;
    event->mode = mode;
    event->preset = preset;
    event->sample_count = sample_count;
    memset(event->samples, 0, sizeof(event->samples));
    memcpy(event->samples, samples, sample_count * sizeof(samples[0]));
}

size_t rf_load_demo_events(rf_event_t *events, size_t capacity)
{
    static const rf_sample_t event0[] = {
        {0.0f, 0.0f, 0.0f, false, 0.00f, 0.01f},
        {2.0f, 4.0f, 33.0f, false, 0.05f, 0.03f},
        {4.0f, 8.0f, 66.0f, false, 0.18f, 0.05f},
        {6.0f, 10.0f, 82.0f, true, 1.28f, 0.88f},
        {7.0f, 10.0f, 84.0f, false, 1.10f, 0.52f},
        {8.0f, 10.0f, 85.0f, true, 1.34f, 0.40f},
        {12.0f, 12.0f, 92.0f, true, 1.46f, 0.14f},
        {20.0f, 6.0f, 49.0f, true, 1.43f, 0.05f},
        {24.0f, 4.0f, 31.0f, false, 0.12f, 0.64f},
        {28.0f, 0.0f, 0.0f, false, 0.00f, 0.03f}
    };

    static const rf_sample_t event1[] = {
        {0.0f, 0.0f, 0.0f, false, 0.00f, 0.01f},
        {3.0f, 8.0f, 29.0f, false, 0.04f, 0.02f},
        {6.0f, 14.0f, 51.0f, false, 0.16f, 0.04f},
        {9.0f, 19.0f, 70.0f, false, 0.32f, 0.08f},
        {12.0f, 22.0f, 81.0f, true, 1.12f, 0.71f},
        {13.0f, 22.0f, 83.0f, false, 1.00f, 0.40f},
        {14.0f, 22.0f, 84.0f, true, 1.26f, 0.36f},
        {15.0f, 22.0f, 85.0f, false, 1.08f, 0.24f},
        {16.0f, 22.0f, 84.0f, true, 1.33f, 0.21f},
        {24.0f, 24.0f, 91.0f, true, 1.42f, 0.08f},
        {32.0f, 8.0f, 29.0f, true, 1.40f, 0.03f},
        {36.0f, 5.0f, 18.0f, false, 0.20f, 0.58f},
        {40.0f, 0.0f, 0.0f, false, 0.00f, 0.03f}
    };

    static const rf_sample_t event2[] = {
        {0.0f, 0.0f, 0.0f, false, 0.00f, 0.01f},
        {3.0f, 8.0f, 46.0f, false, 0.06f, 0.03f},
        {6.0f, 12.0f, 69.0f, false, 0.14f, 0.05f},
        {9.0f, 16.0f, 92.0f, false, 0.28f, 0.06f},
        {12.0f, 20.0f, 114.0f, false, 0.52f, 0.09f},
        {15.0f, 24.0f, 134.0f, true, 1.18f, 0.66f},
        {24.0f, 24.0f, 136.0f, true, 1.24f, 0.09f},
        {30.0f, 10.0f, 57.0f, true, 1.18f, 0.03f},
        {36.0f, 2.0f, 11.0f, false, 0.18f, 0.61f},
        {42.0f, 0.0f, 0.0f, false, 0.00f, 0.03f}
    };

    static const rf_sample_t event3[] = {
        {0.0f, 0.0f, 0.0f, false, 0.00f, 0.01f},
        {3.0f, 6.0f, 24.0f, false, 0.03f, 0.02f},
        {6.0f, 9.0f, 36.0f, false, 0.07f, 0.03f},
        {9.0f, 12.0f, 48.0f, false, 0.10f, 0.05f},
        {12.0f, 12.0f, 49.0f, false, 0.11f, 0.04f},
        {16.0f, 0.0f, 0.0f, false, 0.00f, 0.02f}
    };

    if (capacity < 4) {
        return 0;
    }

    fill_event(&events[0], "2026-09-04T02:20:00Z", RF_MODE_PULL_IN_SWEEP, &PRESET_AUTO_12V, event0, sizeof(event0) / sizeof(event0[0]));
    fill_event(&events[1], "2026-09-04T02:21:00Z", RF_MODE_BOUNCE_CAPTURE, &PRESET_PLC_24V, event1, sizeof(event1) / sizeof(event1[0]));
    fill_event(&events[2], "2026-09-04T02:22:00Z", RF_MODE_HOLD_RELEASE_SWEEP, &PRESET_CONTACTOR_AUX, event2, sizeof(event2) / sizeof(event2[0]));
    fill_event(&events[3], "2026-09-04T02:23:00Z", RF_MODE_PULL_IN_SWEEP, &PRESET_AUTO_12V, event3, sizeof(event3) / sizeof(event3[0]));
    return 4;
}
