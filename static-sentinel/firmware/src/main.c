#include "esd_core.h"
#include <stdio.h>

int main(void) {
    const esd_policy_t policy = {7.5e5f, 1.0e9f, 7.5e5f, 3.5e7f, 30.0f};
    const esd_sample_t sample = {1.0e6f, 1.2e6f, 41.0f, true};
    printf("static-sentinel status=%s\n", esd_status_name(esd_evaluate(&sample, &policy)));
    return 0;
}
