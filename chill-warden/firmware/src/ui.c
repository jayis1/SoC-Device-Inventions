#include <stdio.h>

#include "ui.h"

void cw_print_report(const cw_frame_t *frames, const cw_result_t *results, size_t count)
{
    puts("Chill Warden diagnostic simulation");
    puts("=================================");

    for (size_t i = 0; i < count; ++i) {
        printf("asset=%s mode=%s\n", frames[i].asset_id, cw_mode_string(frames[i].mode));
        printf("  suction=%.1fC discharge=%.1fC ambient=%.1fC rh=%.1f%% current=%.1fA\n",
               frames[i].suction_c,
               frames[i].discharge_c,
               frames[i].ambient_c,
               frames[i].rh_pct,
               frames[i].current_a);
        printf("  vibration=%.2fg acoustic=%.1fdB piezo=%.1fdB cycle=%.0fs superheat_proxy=%.1fC split=%.1fC\n",
               frames[i].vibration_g,
               frames[i].acoustic_db,
               frames[i].piezo_db,
               frames[i].cycle_period_s,
               frames[i].superheat_proxy_c,
               frames[i].condenser_split_c);
        printf("  fault=%s risk=%.1f\n", cw_fault_string(results[i].fault), results[i].risk_score);
        printf("  reason=%s\n", results[i].reason);
        printf("  action=%s\n\n", results[i].action);
    }
}
