#include <stddef.h>
#include <string.h>

#include "sensors.h"

size_t ss_load_demo_frames(ss_frame_t *frames, size_t capacity)
{
    static const ss_frame_t demo[] = {
        {
            .label = "normal rain event",
            .level_mm = 445.0f,
            .rise_rate_mm_min = 18.0f,
            .drawdown_rate_mm_min = -120.0f,
            .pump_current_a = 7.4f,
            .vibration_g_rms = 0.26f,
            .humidity_rh = 72.0f,
            .battery_v = 4.06f,
            .rebound_mm_60s = 8.0f,
            .float_active = true,
            .leak_active = false,
            .power_fail = false
        },
        {
            .label = "float up no start",
            .level_mm = 520.0f,
            .rise_rate_mm_min = 22.0f,
            .drawdown_rate_mm_min = -4.0f,
            .pump_current_a = 0.1f,
            .vibration_g_rms = 0.01f,
            .humidity_rh = 74.0f,
            .battery_v = 3.98f,
            .rebound_mm_60s = 0.0f,
            .float_active = true,
            .leak_active = false,
            .power_fail = false
        },
        {
            .label = "failed check valve",
            .level_mm = 310.0f,
            .rise_rate_mm_min = 6.0f,
            .drawdown_rate_mm_min = -118.0f,
            .pump_current_a = 7.1f,
            .vibration_g_rms = 0.23f,
            .humidity_rh = 73.0f,
            .battery_v = 4.02f,
            .rebound_mm_60s = 86.0f,
            .float_active = false,
            .leak_active = false,
            .power_fail = false
        },
        {
            .label = "short cycle",
            .level_mm = 355.0f,
            .rise_rate_mm_min = 11.0f,
            .drawdown_rate_mm_min = -58.0f,
            .pump_current_a = 6.8f,
            .vibration_g_rms = 0.19f,
            .humidity_rh = 78.0f,
            .battery_v = 3.95f,
            .rebound_mm_60s = 54.0f,
            .float_active = true,
            .leak_active = false,
            .power_fail = false
        },
        {
            .label = "dry run",
            .level_mm = 118.0f,
            .rise_rate_mm_min = 0.4f,
            .drawdown_rate_mm_min = -6.0f,
            .pump_current_a = 5.6f,
            .vibration_g_rms = 0.38f,
            .humidity_rh = 69.0f,
            .battery_v = 3.91f,
            .rebound_mm_60s = 2.0f,
            .float_active = false,
            .leak_active = false,
            .power_fail = false
        },
        {
            .label = "partial blockage",
            .level_mm = 470.0f,
            .rise_rate_mm_min = 17.0f,
            .drawdown_rate_mm_min = -26.0f,
            .pump_current_a = 8.2f,
            .vibration_g_rms = 0.30f,
            .humidity_rh = 77.0f,
            .battery_v = 3.88f,
            .rebound_mm_60s = 12.0f,
            .float_active = true,
            .leak_active = false,
            .power_fail = false
        },
        {
            .label = "flood on floor",
            .level_mm = 548.0f,
            .rise_rate_mm_min = 25.0f,
            .drawdown_rate_mm_min = -10.0f,
            .pump_current_a = 0.2f,
            .vibration_g_rms = 0.02f,
            .humidity_rh = 89.0f,
            .battery_v = 3.84f,
            .rebound_mm_60s = 0.0f,
            .float_active = true,
            .leak_active = true,
            .power_fail = true
        },
        {
            .label = "humid standby",
            .level_mm = 250.0f,
            .rise_rate_mm_min = 1.0f,
            .drawdown_rate_mm_min = -2.0f,
            .pump_current_a = 0.0f,
            .vibration_g_rms = 0.00f,
            .humidity_rh = 86.0f,
            .battery_v = 4.10f,
            .rebound_mm_60s = 0.0f,
            .float_active = false,
            .leak_active = false,
            .power_fail = false
        }
    };

    const size_t count = sizeof(demo) / sizeof(demo[0]);
    const size_t limit = (capacity < count) ? capacity : count;

    for (size_t i = 0; i < limit; ++i) {
        frames[i] = demo[i];
    }

    return limit;
}
