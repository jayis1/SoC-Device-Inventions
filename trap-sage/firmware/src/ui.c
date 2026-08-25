#include <stdio.h>

#include "ui.h"

void ts_print_report(const ts_capture_t *captures, const ts_result_t *results, size_t count)
{
    printf("Trap Sage diagnostic simulation\n");
    printf("asset_id,classification,upstream_C,downstream_C,deltaT_C,ultra_rms,burstiness,active_fraction,cycle_rate_cpm,health\n");

    for (size_t i = 0; i < count; ++i) {
        printf("%s,%s,%.1f,%.1f,%.1f,%.3f,%.3f,%.3f,%.2f,%.1f\n",
               captures[i].asset_id,
               ts_fault_class_name(results[i].fault_class),
               captures[i].upstream_temp_c,
               captures[i].downstream_temp_c,
               results[i].delta_t_c,
               results[i].ultrasonic_rms,
               results[i].burstiness,
               results[i].active_fraction,
               results[i].cycle_rate_cpm,
               results[i].health_score);
    }
}
