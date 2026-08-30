#include <stdio.h>

#include "classifier.h"
#include "sensors.h"
#include "ui.h"

int main(void)
{
    pp_frame_t frames[8];
    pp_result_t results[8];
    const size_t count = pp_load_demo_frames(frames, 8U);

    for (size_t i = 0; i < count; ++i) {
        pp_classify_frame(&frames[i], &results[i]);
    }

    pp_print_report(frames, results, count);
    return 0;
}
