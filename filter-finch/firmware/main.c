#include "filter_core.h"

#include <stdio.h>

int main(void) {
    const ff_config_t config = {25.0f, 50.0f, 0.8f, 4.0f, 1.8f, 2.4f};
    const ff_sample_t sample = {96.0f, 24.0f, 23.1f, 48.0f, true, true};
    const ff_result_t result = ff_evaluate(&sample, &config);
    printf("status=%s normalized_dp=%.2fPa loading_ratio=%.2f\n",
           ff_status_name(result.status), result.normalized_filter_dp_pa,
           result.loading_ratio);
    return 0;
}
