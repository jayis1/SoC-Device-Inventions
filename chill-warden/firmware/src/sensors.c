#include "sensors.h"

size_t cw_load_demo_frames(cw_frame_t *frames, size_t max_frames)
{
    static const cw_frame_t demo[] = {
        {"RTU-17", CW_MODE_COOL, 9.8, 67.4, 31.2, 52.0, 8.1, 0.42, 58.0, 34.0, 1620.0, 8.4, 36.2},
        {"FREEZER-4", CW_MODE_FREEZER, -13.2, 61.0, 24.0, 84.0, 7.4, 0.58, 60.2, 38.1, 4200.0, 1.8, 37.0},
        {"HP-22", CW_MODE_HEAT, 18.0, 88.5, 34.5, 48.0, 9.0, 0.61, 71.5, 42.0, 1500.0, 6.0, 54.0},
        {"CU-9", CW_MODE_COOL, 18.5, 54.0, 29.0, 44.0, 4.1, 0.39, 55.2, 31.0, 1800.0, 21.0, 25.0},
        {"HP-3", CW_MODE_HEAT, 14.0, 78.0, 30.0, 57.0, 8.4, 1.92, 76.4, 63.0, 1700.0, 7.0, 48.0},
        {"WKIN-12", CW_MODE_COOL, 3.0, 64.5, 27.0, 65.0, 9.1, 1.20, 66.0, 78.0, 1400.0, 1.1, 37.5},
        {"MINI-5", CW_MODE_COOL, 11.0, 63.5, 28.5, 50.0, 8.0, 0.47, 59.0, 33.0, 180.0, 8.0, 35.0}
    };

    size_t count = sizeof(demo) / sizeof(demo[0]);
    if (count > max_frames) {
        count = max_frames;
    }

    for (size_t i = 0; i < count; ++i) {
        frames[i] = demo[i];
    }

    return count;
}
