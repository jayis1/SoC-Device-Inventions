#include <stdio.h>

#include "relay_finch.h"
#include "sensors.h"
#include "analysis.h"
#include "classifier.h"

static void print_event(const rf_event_t *event,
                        const rf_features_t *features,
                        const rf_decision_t *decision)
{
    printf("%s mode=%s preset=%s pull_in=%.2fV drop_out=%.2fV operate=%.2fms release=%.2fms bounce=%.2fms chatter=%d pickup=%.1fmA hold=%.1fmA R=%.1fohm travel=%.2fmm acoustic=%.2f state=%s action=%s\n",
           event->timestamp,
           rf_mode_name(event->mode),
           event->preset->name,
           features->pull_in_v,
           features->drop_out_v,
           features->operate_ms,
           features->release_ms,
           features->bounce_ms,
           features->chatter_edges,
           features->pickup_current_ma,
           features->hold_current_ma,
           features->coil_resistance_ohm,
           features->max_travel_mm,
           features->acoustic_peak,
           rf_state_name(decision->state),
           decision->recommended_action);
}

int main(void)
{
    rf_event_t events[8];
    size_t count = rf_load_demo_events(events, 8);

    printf("Relay Finch reference firmware demo\n");
    printf("events=%zu\n", count);

    for (size_t i = 0; i < count; ++i) {
        rf_features_t features;
        rf_decision_t decision;
        rf_extract_features(&events[i], &features);
        rf_classify_event(events[i].preset, &features, &decision);
        print_event(&events[i], &features, &decision);
    }

    return 0;
}
