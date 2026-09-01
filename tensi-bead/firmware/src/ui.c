#include <stdio.h>

#include "ui.h"

void tb_print_report(const tb_frame_t *frames, const tb_result_t *results, size_t count)
{
    puts("Tensi Bead demo analysis");
    puts("id  mode      gamma(mN/m)  angle(deg)  qc  classification");
    puts("--  --------  -----------  ----------  --  -------------------------------");

    for (size_t i = 0; i < count; ++i) {
        printf("%02u  %-8s  %11.2f  %10.2f  %s  %s\n",
               frames[i].sample_id,
               tb_mode_name(frames[i].mode),
               results[i].surface_tension_mn_m,
               results[i].contact_angle_deg,
               results[i].qc_ok ? "ok" : "no",
               results[i].classification);
    }
}
