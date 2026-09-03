#include <math.h>
#include <stddef.h>
#include <string.h>

#include "analysis.h"

static float clamp01(float value)
{
    if (value < 0.0f) {
        return 0.0f;
    }
    if (value > 1.0f) {
        return 1.0f;
    }
    return value;
}

void tp_extract_features(const tp_event_t *event, tp_features_t *features)
{
    memset(features, 0, sizeof(*features));
    if (event == NULL || event->sample_count < 3) {
        return;
    }

    size_t peak_index = 0;
    size_t snug_index = 1;
    float accel_sum = 0.0f;

    for (size_t i = 0; i < event->sample_count; ++i) {
        if (event->torque_nm[i] > features->peak_torque_nm) {
            features->peak_torque_nm = event->torque_nm[i];
            peak_index = i;
        }
        accel_sum += event->accel_g[i];
    }

    const size_t prevailing_samples = event->sample_count < 4 ? event->sample_count : 4;
    for (size_t i = 0; i < prevailing_samples; ++i) {
        features->prevailing_torque_nm += event->torque_nm[i];
    }
    features->prevailing_torque_nm /= (float)prevailing_samples;

    float best_slope = -1.0f;
    for (size_t i = 1; i < event->sample_count; ++i) {
        float delta_angle = event->angle_deg[i] - event->angle_deg[i - 1];
        if (delta_angle <= 0.05f) {
            continue;
        }
        float slope = (event->torque_nm[i] - event->torque_nm[i - 1]) / delta_angle;
        if (slope > best_slope && event->torque_nm[i] > features->prevailing_torque_nm + 0.3f) {
            best_slope = slope;
            snug_index = i;
            features->snug_torque_nm = event->torque_nm[i];
        }
    }

    if (features->snug_torque_nm <= 0.0f) {
        snug_index = peak_index > 0 ? peak_index - 1 : 0;
        features->snug_torque_nm = event->torque_nm[snug_index];
    }

    float final_angle = event->angle_deg[event->sample_count - 1];
    float snug_angle = event->angle_deg[snug_index];
    features->angle_after_snug_deg = final_angle - snug_angle;

    if (peak_index > snug_index) {
        float delta_torque = event->torque_nm[peak_index] - event->torque_nm[snug_index];
        float delta_angle = event->angle_deg[peak_index] - event->angle_deg[snug_index];
        if (delta_angle > 0.05f) {
            features->stiffness_nm_per_deg = delta_torque / delta_angle;
        }
    }

    float min_after_peak = event->torque_nm[peak_index];
    float min_angle_after_peak = event->angle_deg[peak_index];
    for (size_t i = peak_index; i < event->sample_count; ++i) {
        if (event->torque_nm[i] < min_after_peak) {
            min_after_peak = event->torque_nm[i];
        }
        if (event->angle_deg[i] < min_angle_after_peak) {
            min_angle_after_peak = event->angle_deg[i];
        }
    }

    features->backoff_deg = event->angle_deg[peak_index] - min_angle_after_peak;

    float tail_drop = 0.0f;
    if (event->sample_count >= 3) {
        float tail_avg = 0.0f;
        size_t tail_start = event->sample_count - 3;
        for (size_t i = tail_start; i < event->sample_count; ++i) {
            tail_avg += event->torque_nm[i];
        }
        tail_avg /= 3.0f;
        tail_drop = features->peak_torque_nm - tail_avg;
    }
    features->yield_risk = clamp01((tail_drop / (features->peak_torque_nm + 0.001f)) * 2.0f);

    float avg_accel = accel_sum / (float)event->sample_count;
    float angle_span = event->angle_deg[event->sample_count - 1] - event->angle_deg[0];
    float friction_ratio = (features->peak_torque_nm > 0.01f)
        ? (features->prevailing_torque_nm / features->peak_torque_nm)
        : 0.0f;
    float motion_penalty = angle_span < 12.0f ? 0.35f : 0.0f;
    float accel_penalty = avg_accel > 0.12f ? 0.20f : 0.0f;
    features->cross_thread_score = clamp01(friction_ratio * 1.4f + motion_penalty + accel_penalty);

    if (event->temperature_c > 25.0f) {
        float temp_correction = 1.0f - ((event->temperature_c - 25.0f) * 0.0015f);
        features->peak_torque_nm *= temp_correction;
        features->snug_torque_nm *= temp_correction;
        features->prevailing_torque_nm *= temp_correction;
    }
}
