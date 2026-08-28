#include <stdio.h>

#include "ui.h"

void pw_print_report(const pw_frame_t *frames, const pw_result_t *results, size_t count)
{
    puts("Panel Warden diagnostic report");
    puts("====================================================================================================================");
    puts("Asset           | Fault               | Risk | Conf | Hotspot | DeltaT | DewMrg | Ultra | RF     | Load | Note");
    puts("--------------------------------------------------------------------------------------------------------------------");

    for (size_t i = 0; i < count; ++i) {
        printf("%-15s | %-19s | %4.1f | %4.2f | %6.1fC | %6.1fC | %6.1fC | %5.1fdB | %6.1fdBm | %4.1fA | %s\n",
               frames[i].asset_id,
               pw_fault_name(results[i].fault),
               results[i].risk_score,
               results[i].confidence,
               frames[i].hotspot_c,
               results[i].delta_t_c,
               results[i].dew_margin_c,
               frames[i].ultrasound_db,
               frames[i].rf_dbm_equiv,
               frames[i].load_corr_a,
               results[i].note);
    }
}
