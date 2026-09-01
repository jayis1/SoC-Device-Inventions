#include <stdio.h>

#include "analysis.h"
#include "sensors.h"
#include "ui.h"

int main(void)
{
    tb_frame_t frames[8];
    tb_result_t results[8];
    const size_t count = tb_load_demo_frames(frames, 8U);

    for (size_t i = 0; i < count; ++i) {
        tb_analyze_frame(&frames[i], &results[i]);
    }

    tb_print_report(frames, results, count);
    return 0;
}
