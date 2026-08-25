#include <stddef.h>

#include "classifier.h"
#include "sensors.h"
#include "ui.h"

int main(void)
{
    ts_capture_t captures[5];
    ts_result_t results[5];
    size_t count = ts_load_demo_captures(captures, 5U);

    for (size_t i = 0; i < count; ++i) {
        ts_classify_capture(&captures[i], &results[i]);
    }

    ts_print_report(captures, results, count);
    return 0;
}
