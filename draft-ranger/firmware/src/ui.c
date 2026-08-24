#include <stdio.h>

#include "ui.h"
#include "classifier.h"

void dr_print_report(const dr_capture_t *captures, const dr_result_t *results, size_t count)
{
    printf("Draft Ranger simulated field sweep\n");
    printf("=================================\n\n");

    for (size_t i = 0; i < count; ++i) {
        printf("Mark %d\n", captures[i].mark_index);
        printf("  Pressure            : %6.2f Pa\n", captures[i].pressure_pa);
        printf("  Hot-wire bridge     : %6.3f V\n", captures[i].bridge_v);
        printf("  Ambient             : %5.2f C, %5.1f %%RH\n",
               captures[i].ambient_temp_c,
               captures[i].ambient_rh_pct);
        printf("  Surface temps       : frame %5.2f C | adjacent %5.2f C\n",
               captures[i].frame_temp_c,
               captures[i].surface_temp_c);
        printf("  Dew point           : %5.2f C\n", results[i].dew_point_c);
        printf("  Condensation margin : %5.2f C\n", results[i].condensation_margin_c);
        printf("  Draft velocity      : %5.2f m/s\n", results[i].draft_velocity_mps);
        printf("  Whistle             : %4.0f Hz, score %.3f\n",
               results[i].whistle_hz,
               results[i].whistle_score);
        printf("  Leak severity       : %5.1f / 100\n", results[i].leak_severity);
        printf("  Seal health         : %5.1f / 100\n", results[i].seal_health);
        printf("  Fault class         : %s\n\n", dr_fault_class_name(results[i].fault_class));
    }
}
