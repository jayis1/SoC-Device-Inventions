#include <stdio.h>

#include "classifier.h"
#include "sensors.h"
#include "ui.h"

int main(void)
{
    ss_frame_t frames[8];
    ss_result_t results[8];
    const size_t count = ss_load_demo_frames(frames, 8U);

    for (size_t i = 0; i < count; ++i) {
        ss_classify_frame(&frames[i], &results[i]);
    }

    ss_print_report(frames, results, count);
    return 0;
}
