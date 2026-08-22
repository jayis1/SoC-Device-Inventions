#include <stdio.h>

#include "pipe_halo.h"
#include "sensors.h"
#include "dsp.h"
#include "classifier.h"

static void print_window(const ph_sensor_window_t *window,
                         const ph_features_t *features,
                         const ph_decision_t *decision)
{
    printf("%s,state=%s,flow=%.3f,hammer=%.3f,drip=%.3f,freeze_margin=%.3f,burst=%.3f,action=%s\n",
           window->timestamp,
           ph_state_name(decision->state),
           features->flow_index,
           features->hammer_index,
           features->drip_confidence,
           features->freeze_margin_c,
           features->burst_risk_score,
           decision->recommended_action);
}

int main(void)
{
    ph_sensor_window_t windows[8];
    size_t count = ph_load_demo_windows(windows, 8);

    printf("Pipe Halo reference firmware demo\n");
    printf("windows=%zu\n", count);

    for (size_t i = 0; i < count; ++i) {
        ph_features_t features;
        ph_decision_t decision;
        ph_extract_features(&windows[i], &features);
        ph_classify_window(&windows[i], &features, &decision);
        print_window(&windows[i], &features, &decision);
    }

    return 0;
}
