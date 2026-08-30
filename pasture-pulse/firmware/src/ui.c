#include <stdio.h>

#include "ui.h"

void pp_print_report(const pp_frame_t *frames, const pp_result_t *results, size_t count)
{
    puts("Pasture Pulse host simulation");
    puts("============================");

    for (size_t i = 0; i < count; ++i) {
        const pp_frame_t *frame = &frames[i];
        const pp_result_t *result = &results[i];
        printf("\nScenario: %s\n", frame->scenario_name);
        printf("  Fault: %s (confidence %.2f)\n", pp_fault_name(result->fault), result->confidence);
        printf("  Pulse: %.2f kV peak, %.2f A peak, %.2f s interval\n",
               frame->pulse_peak_kv,
               frame->current_peak_a,
               frame->pulse_interval_s);
        printf("  Features: energy_index=%.2f leakage_ratio=%.2f wetness=%.2f fence_score=%.1f\n",
               result->features.energy_index,
               result->features.leakage_ratio,
               result->features.wetness_factor,
               result->features.fence_score);
        printf("  Summary: %s\n", result->summary);

        for (size_t note = 0; note < PP_MAX_NOTES; ++note) {
            if (result->notes[note] != NULL) {
                printf("  Note %zu: %s\n", note + 1U, result->notes[note]);
            }
        }
    }
}
