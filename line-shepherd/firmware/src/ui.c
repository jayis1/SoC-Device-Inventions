#include <stdio.h>

#include "ui.h"
#include "tdr.h"

void ls_print_report(const ls_capture_t *captures, const ls_result_t *results, size_t count)
{
    printf("Line Shepherd firmware simulation\n");
    printf("%-18s %-7s %-14s %8s %8s %8s %8s\n",
           "Asset", "Pair", "Fault", "Dist(m)", "Health", "Conf", "Zhint");

    for (size_t i = 0; i < count; ++i) {
        printf("%-18s %-7s %-14s %8.1f %8.1f %8.2f %8.1f\n",
               captures[i].asset_id,
               captures[i].pair_name,
               ls_fault_name(results[i].fault),
               results[i].est_distance_m,
               results[i].health_score,
               results[i].confidence,
               results[i].impedance_hint_ohm);
    }

    printf("\nDetailed notes:\n");
    for (size_t i = 0; i < count; ++i) {
        printf("- %s: %s (remote_id=%d, cable=%s, loop_res=%.1f ohm, cap=%.1f nF)\n",
               captures[i].asset_id,
               results[i].notes,
               captures[i].remote_id,
               captures[i].cable_type,
               captures[i].loop_res_ohm,
               captures[i].cap_nf);
    }
}
