#include "sensors.h"
#include "porometer.h"
#include "ui.h"

int main(void)
{
    sc_capture_t capture;
    sc_result_t result;

    sc_make_demo_capture(&capture);
    sc_compute_result(&capture, &result);
    sc_print_report(&capture, &result);

    return 0;
}
