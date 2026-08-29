#include <stdio.h>

#include "classifier.h"
#include "sensors.h"
#include "ui.h"

int main(void)
{
    cw_frame_t frames[8];
    cw_result_t results[8];
    const size_t count = cw_load_demo_frames(frames, 8U);

    for (size_t i = 0; i < count; ++i) {
        cw_classify_frame(&frames[i], &results[i]);
    }

    cw_print_report(frames, results, count);
    return 0;
}
