#include <stdio.h>

#include "classifier.h"
#include "sensors.h"
#include "ui.h"

int main(void)
{
    ls_capture_t captures[8];
    ls_result_t results[8];
    const size_t count = ls_load_demo_captures(captures, 8U);

    for (size_t i = 0; i < count; ++i) {
        ls_classify_capture(&captures[i], &results[i]);
    }

    ls_print_report(captures, results, count);
    return 0;
}
