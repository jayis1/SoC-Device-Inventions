#include <stdio.h>

#include "ui.h"

void ss_print_report(const ss_frame_t *frames, const ss_result_t *results, size_t count)
{
    printf("Sump Shepherd firmware simulation\n");
    printf("===============================================================\n");
    printf("%-18s %-16s %8s %8s %8s %8s\n",
           "scenario",
           "state",
           "level",
           "current",
           "humid",
           "risk");

    for (size_t i = 0; i < count; ++i) {
        printf("%-18s %-16s %8.1f %8.2f %8.1f %8.1f\n",
               frames[i].label,
               ss_state_name(results[i].state),
               frames[i].level_mm,
               frames[i].pump_current_a,
               frames[i].humidity_rh,
               results[i].flood_risk_score);
    }
}
