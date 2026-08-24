#include <stddef.h>

#include "classifier.h"
#include "sensors.h"
#include "ui.h"

int main(void)
{
    dr_capture_t captures[4];
    dr_result_t results[4];
    size_t count = dr_load_demo_captures(captures, 4U);

    for (size_t i = 0; i < count; ++i) {
        dr_classify_capture(&captures[i], &results[i]);
    }

    dr_print_report(captures, results, count);
    return 0;
}
