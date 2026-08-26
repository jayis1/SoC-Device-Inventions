#include <stddef.h>

#include "classifier.h"
#include "sensors.h"
#include "ui.h"

int main(void)
{
    cc_capture_t captures[4];
    cc_result_t results[4];
    size_t count = cc_load_demo_captures(captures, 4U);

    for (size_t i = 0; i < count; ++i) {
        cc_classify_capture(&captures[i], &results[i]);
    }

    cc_print_report(captures, results, count);
    return 0;
}
