#include <stdio.h>

#include "torque_pebble.h"
#include "sensors.h"
#include "analysis.h"
#include "classifier.h"

static void print_event(const tp_event_t *event,
                        const tp_features_t *features,
                        const tp_decision_t *decision)
{
    printf("%s,preset=%s,peak=%.2f,snug=%.2f,angle=%.1f,prevailing=%.2f,stiffness=%.3f,backoff=%.1f,yield=%.2f,cross=%.2f,state=%s,action=%s\n",
           event->timestamp,
           event->preset->name,
           features->peak_torque_nm,
           features->snug_torque_nm,
           features->angle_after_snug_deg,
           features->prevailing_torque_nm,
           features->stiffness_nm_per_deg,
           features->backoff_deg,
           features->yield_risk,
           features->cross_thread_score,
           tp_state_name(decision->state),
           decision->recommended_action);
}

int main(void)
{
    tp_event_t events[8];
    size_t count = tp_load_demo_events(events, 8);

    printf("Torque Pebble reference firmware demo\n");
    printf("events=%zu\n", count);

    for (size_t i = 0; i < count; ++i) {
        tp_features_t features;
        tp_decision_t decision;
        tp_extract_features(&events[i], &features);
        tp_classify_event(&events[i], &features, &decision);
        print_event(&events[i], &features, &decision);
    }

    return 0;
}
