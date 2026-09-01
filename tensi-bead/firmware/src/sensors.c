#include "sensors.h"

const char *tb_mode_name(tb_mode_t mode)
{
    return mode == TB_MODE_PENDANT ? "pendant" : "sessile";
}

size_t tb_load_demo_frames(tb_frame_t *frames, size_t max_frames)
{
    static const tb_frame_t demo[] = {
        {1U, TB_MODE_PENDANT, 0.0215f, 22.0f, 68.0f, 86.0f, 0.0f, 0.0f, 0.0f, 0.998f, 23.4f, 45.0f, 38.2f, 810.0f},
        {2U, TB_MODE_PENDANT, 0.0215f, 20.0f, 64.0f, 92.0f, 0.0f, 0.0f, 0.0f, 0.997f, 24.1f, 46.0f, 34.8f, 805.0f},
        {3U, TB_MODE_PENDANT, 0.0215f, 17.0f, 58.0f, 94.0f, 0.0f, 0.0f, 0.0f, 0.930f, 24.0f, 43.0f, 30.1f, 790.0f},
        {4U, TB_MODE_PENDANT, 0.0215f, 15.0f, 50.0f, 79.0f, 0.0f, 0.0f, 0.0f, 0.790f, 23.8f, 42.0f, 18.6f, 780.0f},
        {5U, TB_MODE_SESSILE, 0.0200f, 0.0f, 0.0f, 28.0f, 88.0f, 37.0f, 41.0f, 1.000f, 23.7f, 44.0f, 12.0f, 820.0f},
        {6U, TB_MODE_SESSILE, 0.0200f, 0.0f, 0.0f, 34.0f, 66.0f, 82.0f, 86.0f, 0.998f, 23.9f, 45.0f, 11.6f, 818.0f},
        {7U, TB_MODE_SESSILE, 0.0200f, 0.0f, 0.0f, 30.0f, 74.0f, 56.0f, 60.0f, 1.010f, 24.3f, 47.0f, 13.1f, 812.0f},
        {8U, TB_MODE_PENDANT, 0.0215f, 12.0f, 46.0f, 96.0f, 0.0f, 0.0f, 0.0f, 1.120f, 22.7f, 48.0f, 29.5f, 795.0f}
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
