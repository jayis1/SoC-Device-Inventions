#include "ui.h"

#include <stdio.h>

void cc_print_report(const cc_capture_t *captures, const cc_result_t *results, size_t count)
{
    puts("Chloro Clip fluorescence summary");
    puts("leaf_id,tempC,PAR,Fo,Fm,Fv/Fm,Fs,Fm',Fo',PhiPSII,NPQ,qP,stress,diagnosis");

    for (size_t i = 0; i < count; ++i) {
        printf("%s,%.1f,%.0f,%.1f,%.1f,%.3f,%.1f,%.1f,%.1f,%.3f,%.3f,%.3f,%.1f,%s\n",
               captures[i].leaf_id,
               captures[i].leaf_temp_c,
               captures[i].ambient_par_umol,
               results[i].fo,
               results[i].fm,
               results[i].fv_fm,
               results[i].fs,
               results[i].fm_prime,
               results[i].fo_prime,
               results[i].phi_psii,
               results[i].npq,
               results[i].qp,
               results[i].stress_index,
               cc_diagnosis_str(results[i].diagnosis));
    }
}
