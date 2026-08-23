#include <stdio.h>

#include "ui.h"
#include "porometer.h"

void sc_print_report(const sc_capture_t *capture, const sc_result_t *result)
{
    printf("Stoma Clip demo measurement\n");
    printf("leaf temp: %.2f C\n", capture->leaf_temp_c);
    printf("ambient temp: %.2f C\n", capture->ambient_temp_c);
    printf("ambient RH: %.2f %%\n", capture->ambient_rh_pct);
    printf("leaf-air delta: %.2f C\n", result->leaf_air_delta_c);
    printf("VPD: %.2f kPa\n", result->vpd_kpa);
    printf("vapor slope: %.4f g m^-3 s^-1\n", result->vapor_slope_g_m3_s);
    printf("transpiration proxy: %.4f g m^-2 s^-1\n", result->transpiration_g_m2_s);
    printf("stomatal conductance: %.3f mol m^-2 s^-1\n", result->stomatal_conductance_mol_m2_s);
    printf("stress score: %.1f / 100\n", result->stress_score);
    printf("condition: %s\n", sc_condition_name(result->condition));
    printf("fit r^2: %.3f\n", result->fit_r2);
}
