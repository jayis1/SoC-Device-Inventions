#include "sensors.h"

#include <string.h>

static void copy_curve(float *dst, const float *src)
{
    for (size_t i = 0; i < CC_CURVE_SAMPLES; ++i) {
        dst[i] = src[i];
    }
}

size_t cc_load_demo_captures(cc_capture_t *captures, size_t capacity)
{
    static const cc_capture_t demo[] = {
        {
            "oak-healthy-01", 28.4f, 27.1f, 780.0f, 3.96f,
            {118.0f, 120.0f, 126.0f, 312.0f, 588.0f, 742.0f, 781.0f, 776.0f, 768.0f, 402.0f, 196.0f, 138.0f},
            {428.0f, 431.0f, 438.0f, 544.0f, 636.0f, 671.0f, 666.0f, 658.0f, 612.0f, 520.0f, 410.0f, 156.0f}
        },
        {
            "pepper-drought-02", 34.7f, 31.5f, 1010.0f, 3.91f,
            {132.0f, 135.0f, 139.0f, 276.0f, 468.0f, 598.0f, 636.0f, 628.0f, 624.0f, 352.0f, 184.0f, 146.0f},
            {396.0f, 401.0f, 406.0f, 471.0f, 518.0f, 548.0f, 544.0f, 541.0f, 502.0f, 430.0f, 336.0f, 168.0f}
        },
        {
            "lettuce-heat-03", 39.2f, 35.8f, 1250.0f, 3.88f,
            {146.0f, 148.0f, 150.0f, 244.0f, 352.0f, 418.0f, 446.0f, 441.0f, 438.0f, 264.0f, 182.0f, 158.0f},
            {332.0f, 338.0f, 340.0f, 376.0f, 404.0f, 428.0f, 426.0f, 423.0f, 392.0f, 348.0f, 296.0f, 184.0f}
        },
        {
            "corn-lowN-04", 30.1f, 29.4f, 920.0f, 3.90f,
            {126.0f, 127.0f, 130.0f, 296.0f, 514.0f, 678.0f, 724.0f, 718.0f, 710.0f, 382.0f, 192.0f, 140.0f},
            {488.0f, 492.0f, 498.0f, 552.0f, 596.0f, 624.0f, 620.0f, 616.0f, 582.0f, 516.0f, 426.0f, 164.0f}
        }
    };

    size_t available = sizeof(demo) / sizeof(demo[0]);
    size_t count = (capacity < available) ? capacity : available;

    for (size_t i = 0; i < count; ++i) {
        memset(&captures[i], 0, sizeof(captures[i]));
        (void)strncpy(captures[i].leaf_id, demo[i].leaf_id, CC_LEAF_ID_LEN - 1U);
        captures[i].leaf_temp_c = demo[i].leaf_temp_c;
        captures[i].ambient_temp_c = demo[i].ambient_temp_c;
        captures[i].ambient_par_umol = demo[i].ambient_par_umol;
        captures[i].battery_v = demo[i].battery_v;
        copy_curve(captures[i].dark_curve, demo[i].dark_curve);
        copy_curve(captures[i].light_curve, demo[i].light_curve);
    }

    return count;
}
