#include <stddef.h>

#include "sensors.h"

static const tp_preset_t g_presets[] = {
    {"m5_cover_screw", 4.5f, 4.1f, 4.9f, 35.0f, 10.0f, 1},
    {"m6_frame_bolt", 9.0f, 8.4f, 9.6f, 55.0f, 12.0f, 1},
    {"battery_lug", 6.0f, 5.5f, 6.5f, 20.0f, 8.0f, 1},
    {"bike_stem_faceplate", 5.5f, 5.2f, 5.8f, 0.0f, 0.0f, 0},
};

const tp_preset_t *tp_default_presets(size_t *count)
{
    if (count != NULL) {
        *count = sizeof(g_presets) / sizeof(g_presets[0]);
    }
    return g_presets;
}

const char *tp_state_name(tp_state_t state)
{
    switch (state) {
    case TP_STATE_PASS:
        return "PASS";
    case TP_STATE_UNDER_TORQUE:
        return "UNDER_TORQUE";
    case TP_STATE_OVER_TORQUE:
        return "OVER_TORQUE";
    case TP_STATE_SOFT_JOINT:
        return "SOFT_JOINT";
    case TP_STATE_CROSS_THREAD_RISK:
        return "CROSS_THREAD_RISK";
    case TP_STATE_REWORK_DETECTED:
        return "REWORK_DETECTED";
    default:
        return "UNKNOWN";
    }
}

size_t tp_load_demo_events(tp_event_t *events, size_t max_events)
{
    size_t preset_count = 0;
    const tp_preset_t *presets = tp_default_presets(&preset_count);
    (void)preset_count;

    static const float torque_pass[] = {0.0f, 0.3f, 0.5f, 0.7f, 1.1f, 1.8f, 3.0f, 4.2f, 5.0f, 6.3f, 7.5f, 8.4f, 8.9f, 9.2f, 9.1f, 9.0f};
    static const float angle_pass[]  = {0.0f, 4.0f, 8.5f, 13.0f, 18.0f, 24.0f, 30.0f, 36.0f, 41.0f, 46.0f, 56.0f, 68.0f, 79.0f, 89.0f, 97.0f, 101.2f};
    static const float accel_pass[]  = {0.02f, 0.03f, 0.05f, 0.03f, 0.04f, 0.05f, 0.05f, 0.06f, 0.07f, 0.08f, 0.08f, 0.09f, 0.08f, 0.07f, 0.05f, 0.03f};

    static const float torque_under[] = {0.0f, 0.2f, 0.4f, 0.6f, 0.9f, 1.4f, 2.1f, 2.9f, 3.8f, 4.4f, 4.8f, 5.1f, 5.2f, 5.1f};
    static const float angle_under[]  = {0.0f, 3.0f, 6.0f, 9.0f, 12.0f, 16.0f, 19.0f, 22.0f, 24.0f, 25.5f, 26.0f, 26.5f, 26.8f, 27.0f};
    static const float accel_under[]  = {0.02f, 0.02f, 0.03f, 0.03f, 0.04f, 0.04f, 0.05f, 0.05f, 0.05f, 0.04f, 0.04f, 0.03f, 0.03f, 0.02f};

    static const float torque_soft[] = {0.0f, 0.3f, 0.6f, 0.9f, 1.1f, 1.5f, 2.2f, 3.0f, 3.8f, 4.7f, 5.7f, 6.6f, 7.3f, 8.0f, 8.5f, 8.9f, 9.1f, 9.2f};
    static const float angle_soft[]  = {0.0f, 6.0f, 12.0f, 18.0f, 24.0f, 32.0f, 40.0f, 49.0f, 58.0f, 67.0f, 76.0f, 90.0f, 104.0f, 118.0f, 132.0f, 144.0f, 150.0f, 154.0f};
    static const float accel_soft[]  = {0.03f, 0.03f, 0.04f, 0.04f, 0.04f, 0.05f, 0.05f, 0.06f, 0.07f, 0.07f, 0.07f, 0.08f, 0.08f, 0.08f, 0.07f, 0.06f, 0.05f, 0.04f};

    static const float torque_cross[] = {0.0f, 0.8f, 1.6f, 2.3f, 2.9f, 3.5f, 4.0f, 4.6f, 5.1f, 5.5f, 5.7f, 5.9f, 6.0f, 6.1f};
    static const float angle_cross[]  = {0.0f, 1.0f, 2.0f, 3.0f, 4.2f, 5.0f, 5.8f, 6.5f, 7.2f, 7.8f, 8.2f, 8.5f, 8.8f, 9.0f};
    static const float accel_cross[]  = {0.05f, 0.08f, 0.10f, 0.12f, 0.14f, 0.11f, 0.16f, 0.15f, 0.17f, 0.18f, 0.14f, 0.16f, 0.15f, 0.13f};

    static const float torque_rework[] = {0.0f, 0.3f, 0.6f, 1.0f, 1.8f, 2.6f, 3.4f, 4.3f, 5.0f, 5.5f, 5.8f, 5.6f, 5.2f, 5.0f, 5.4f, 5.6f};
    static const float angle_rework[]  = {0.0f, 2.0f, 4.5f, 7.0f, 9.5f, 12.0f, 14.0f, 16.0f, 18.0f, 19.5f, 20.5f, 19.2f, 17.8f, 16.8f, 18.0f, 19.0f};
    static const float accel_rework[]  = {0.03f, 0.04f, 0.05f, 0.05f, 0.06f, 0.07f, 0.07f, 0.08f, 0.08f, 0.09f, 0.10f, 0.15f, 0.16f, 0.14f, 0.10f, 0.08f};

    if (max_events < 5) {
        return 0;
    }

    events[0] = (tp_event_t){
        .timestamp = "2026-09-03T02:15:14Z",
        .preset = &presets[1],
        .sample_count = sizeof(torque_pass) / sizeof(torque_pass[0]),
        .temperature_c = 24.7f,
    };
    events[1] = (tp_event_t){
        .timestamp = "2026-09-03T02:16:02Z",
        .preset = &presets[2],
        .sample_count = sizeof(torque_under) / sizeof(torque_under[0]),
        .temperature_c = 25.1f,
    };
    events[2] = (tp_event_t){
        .timestamp = "2026-09-03T02:17:41Z",
        .preset = &presets[1],
        .sample_count = sizeof(torque_soft) / sizeof(torque_soft[0]),
        .temperature_c = 25.4f,
    };
    events[3] = (tp_event_t){
        .timestamp = "2026-09-03T02:19:08Z",
        .preset = &presets[0],
        .sample_count = sizeof(torque_cross) / sizeof(torque_cross[0]),
        .temperature_c = 26.0f,
    };
    events[4] = (tp_event_t){
        .timestamp = "2026-09-03T02:21:55Z",
        .preset = &presets[3],
        .sample_count = sizeof(torque_rework) / sizeof(torque_rework[0]),
        .temperature_c = 24.9f,
    };

    for (size_t i = 0; i < events[0].sample_count; ++i) {
        events[0].torque_nm[i] = torque_pass[i];
        events[0].angle_deg[i] = angle_pass[i];
        events[0].accel_g[i] = accel_pass[i];
    }
    for (size_t i = 0; i < events[1].sample_count; ++i) {
        events[1].torque_nm[i] = torque_under[i];
        events[1].angle_deg[i] = angle_under[i];
        events[1].accel_g[i] = accel_under[i];
    }
    for (size_t i = 0; i < events[2].sample_count; ++i) {
        events[2].torque_nm[i] = torque_soft[i];
        events[2].angle_deg[i] = angle_soft[i];
        events[2].accel_g[i] = accel_soft[i];
    }
    for (size_t i = 0; i < events[3].sample_count; ++i) {
        events[3].torque_nm[i] = torque_cross[i];
        events[3].angle_deg[i] = angle_cross[i];
        events[3].accel_g[i] = accel_cross[i];
    }
    for (size_t i = 0; i < events[4].sample_count; ++i) {
        events[4].torque_nm[i] = torque_rework[i];
        events[4].angle_deg[i] = angle_rework[i];
        events[4].accel_g[i] = accel_rework[i];
    }

    return 5;
}
