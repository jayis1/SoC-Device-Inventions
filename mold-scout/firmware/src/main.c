#include <stdio.h>

#include "mold_scout.h"
#include "sensors.h"
#include "dsp.h"
#include "classifier.h"

static void print_window(const ms_sensor_window_t *window,
                         const ms_features_t *features,
                         const ms_decision_t *decision)
{
    printf("%s,state=%s,dew_margin=%.2f,moisture=%.3f,condensation=%.3f,thermal=%.3f,voc=%.3f,mold_risk=%.3f,action=%s\n",
           window->timestamp,
           ms_state_name(decision->state),
           features->dewpoint_margin_c,
           features->moisture_proxy,
           features->condensation_risk,
           features->thermal_bridge_score,
           features->voc_risk,
           features->mold_risk_score,
           decision->recommended_action);
}

int main(void)
{
    ms_sensor_window_t windows[8];
    size_t count = ms_load_demo_windows(windows, 8);

    printf("Mold Scout reference firmware demo\n");
    printf("windows=%zu\n", count);

    for (size_t i = 0; i < count; ++i) {
        ms_features_t features;
        ms_decision_t decision;
        ms_extract_features(&windows[i], &features);
        ms_classify_window(&windows[i], &features, &decision);
        print_window(&windows[i], &features, &decision);
    }

    return 0;
}
