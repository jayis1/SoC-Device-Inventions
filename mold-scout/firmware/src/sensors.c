#include "sensors.h"

size_t ms_load_demo_windows(ms_sensor_window_t *windows, size_t max_windows)
{
    static const ms_sensor_window_t demo[] = {
        {"2026-09-02T02:10:00Z", 23.8f, 51.0f, 22.1f, -1.0f, 47.5f, -6.0f, 95.0f, 42.0f, 0.03f, 4.03f},
        {"2026-09-02T02:11:00Z", 23.7f, 59.5f, 19.4f, -3.5f, 35.1f, -16.5f, 160.0f, 24.0f, 0.04f, 3.98f},
        {"2026-09-02T02:12:00Z", 23.6f, 64.0f, 18.5f, -4.2f, 29.0f, -20.0f, 188.0f, 19.5f, 0.05f, 3.94f},
        {"2026-09-02T02:13:00Z", 23.5f, 67.0f, 17.8f, -5.0f, 24.2f, -25.0f, 225.0f, 15.0f, 0.05f, 3.91f},
        {"2026-09-02T02:14:00Z", 23.4f, 58.2f, 18.9f, -3.7f, 31.2f, -18.4f, 182.0f, 18.7f, 0.05f, 3.88f}
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
