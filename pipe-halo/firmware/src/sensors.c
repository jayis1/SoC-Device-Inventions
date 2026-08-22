#include "sensors.h"

size_t ph_load_demo_windows(ph_sensor_window_t *windows, size_t max_windows)
{
    static const ph_sensor_window_t demo[] = {
        {"2026-08-22T00:00:00Z", 21.2f, 21.0f, 22.0f, 42.0f, 0.004f, 8.0f, 120.0f, 0.0f, 0.10f, 89.0f, 3.28f},
        {"2026-08-22T00:05:00Z", 18.4f, 16.7f, 21.4f, 45.0f, 0.052f, 76.0f, 610.0f, 0.0f, 0.35f, 88.0f, 3.27f},
        {"2026-08-22T00:10:00Z", 8.4f, 6.6f, 11.2f, 74.0f, 0.028f, 42.0f, 260.0f, 0.92f, 0.18f, 87.0f, 3.25f},
        {"2026-08-22T00:15:00Z", 12.0f, 11.7f, 20.4f, 40.0f, 0.210f, 155.0f, 930.0f, 0.0f, 2.80f, 86.0f, 3.24f},
        {"2026-08-22T00:20:00Z", 1.3f, 0.8f, -1.4f, 58.0f, 0.006f, 11.0f, 90.0f, 0.0f, 0.12f, 89.0f, 3.23f},
        {"2026-08-22T00:25:00Z", -0.2f, -0.5f, -4.1f, 92.0f, 0.180f, 140.0f, 820.0f, 0.0f, 3.90f, 20.0f, 3.21f}
    };

    size_t count = sizeof(demo) / sizeof(demo[0]);
    if (count > max_windows) {
        count = max_windows;
    }

    for (size_t i = 0; i < count; ++i) {
        windows[i] = demo[i];
    }

    return count;
}
