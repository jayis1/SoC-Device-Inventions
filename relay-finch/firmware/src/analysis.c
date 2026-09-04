#include <string.h>
#include <math.h>

#include "analysis.h"

static int first_actuated_index(const rf_event_t *event)
{
    for (size_t i = 0; i < event->sample_count; ++i) {
        if (event->samples[i].contact_closed ||
            event->samples[i].hall_mm >= (event->preset->min_travel_mm * RF_TRAVEL_ACTUATED_RATIO)) {
            return (int)i;
        }
    }
    return -1;
}

void rf_extract_features(const rf_event_t *event, rf_features_t *features)
{
    memset(features, 0, sizeof(*features));
    features->drop_out_v = -1.0f;
    features->operate_ms = -1.0f;
    features->release_ms = -1.0f;

    const int act_idx = first_actuated_index(event);
    int release_idx = -1;
    float bounce_window_end_ms = 0.0f;
    bool seen_voltage = false;
    float last_on_voltage = 0.0f;
    float last_on_current = 0.0f;
    float hold_sum = 0.0f;
    int hold_count = 0;

    for (size_t i = 0; i < event->sample_count; ++i) {
        const rf_sample_t *s = &event->samples[i];

        if (s->acoustic > features->acoustic_peak) {
            features->acoustic_peak = s->acoustic;
        }
        if (s->hall_mm > features->max_travel_mm) {
            features->max_travel_mm = s->hall_mm;
        }

        if (s->coil_v > 0.1f) {
            seen_voltage = true;
            last_on_voltage = s->coil_v;
            last_on_current = s->current_ma;
        }

        if (act_idx >= 0 && (int)i >= act_idx && s->contact_closed) {
            hold_sum += s->current_ma;
            ++hold_count;
        }
    }

    features->actuated = (act_idx >= 0);

    if (act_idx >= 0) {
        const rf_sample_t *s = &event->samples[act_idx];
        features->pull_in_v = s->coil_v;
        features->operate_ms = s->time_ms - event->samples[0].time_ms;
        features->pickup_current_ma = s->current_ma;
        bounce_window_end_ms = s->time_ms + 10.0f;

        bool stable_state = s->contact_closed;
        float first_edge_time = s->time_ms;
        float last_edge_time = s->time_ms;
        int chatter_edges = 0;
        for (size_t i = (size_t)act_idx + 1; i < event->sample_count; ++i) {
            if (event->samples[i].time_ms - first_edge_time > 10.0f) {
                break;
            }
            if (event->samples[i].contact_closed != stable_state) {
                stable_state = event->samples[i].contact_closed;
                last_edge_time = event->samples[i].time_ms;
                ++chatter_edges;
            }
        }
        features->chatter_edges = chatter_edges;
        features->bounce_ms = (chatter_edges > 0) ? (last_edge_time - first_edge_time) : 0.0f;
    }

    if (hold_count > 0) {
        features->hold_current_ma = hold_sum / (float)hold_count;
    } else {
        features->hold_current_ma = last_on_current;
    }

    if (last_on_current > 1.0f) {
        features->coil_resistance_ohm = last_on_voltage / (last_on_current / 1000.0f);
    }

    for (size_t i = 1; i < event->sample_count; ++i) {
        const rf_sample_t *prev = &event->samples[i - 1];
        const rf_sample_t *cur = &event->samples[i];
        if (features->actuated &&
            prev->time_ms >= bounce_window_end_ms &&
            prev->contact_closed && !cur->contact_closed) {
            release_idx = (int)i;
            features->drop_out_v = cur->coil_v;
            features->release_ms = cur->time_ms - prev->time_ms;
            features->contact_released = true;
            break;
        }
    }

    if (features->actuated && !features->contact_released && seen_voltage) {
        const rf_sample_t *last = &event->samples[event->sample_count - 1];
        if (last->contact_closed) {
            features->drop_out_v = 0.0f;
        }
    }

    if (release_idx >= 0) {
        for (size_t i = (size_t)release_idx; i < event->sample_count; ++i) {
            if (event->samples[i].hall_mm < 0.2f * event->preset->min_travel_mm) {
                features->release_ms = event->samples[i].time_ms - event->samples[release_idx - 1].time_ms;
                break;
            }
        }
    }

    if (!features->actuated) {
        features->pull_in_v = NAN;
        features->pickup_current_ma = last_on_current;
        features->hold_current_ma = 0.0f;
        features->bounce_ms = 0.0f;
        features->chatter_edges = 0;
    }
}
