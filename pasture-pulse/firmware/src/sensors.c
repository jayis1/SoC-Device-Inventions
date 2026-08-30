#include "sensors.h"

size_t pp_load_demo_frames(pp_frame_t *frames, size_t max_frames)
{
    static const pp_frame_t demo[] = {
        {
            .scenario_name = "North pasture baseline",
            .pulse_peak_kv = 7.4,
            .rise_time_us = 18.0,
            .decay_time_us = 145.0,
            .pulse_interval_s = 1.03,
            .current_peak_a = 2.1,
            .ambient_temp_c = 21.0,
            .humidity_rh = 46.0,
            .dewpoint_margin_c = 9.2,
            .battery_v = 3.33,
            .gate_open = 0,
            .surge_flag = 0,
            .pulse_missing = 0
        },
        {
            .scenario_name = "South line grass contact",
            .pulse_peak_kv = 5.1,
            .rise_time_us = 24.0,
            .decay_time_us = 244.0,
            .pulse_interval_s = 1.02,
            .current_peak_a = 4.6,
            .ambient_temp_c = 27.0,
            .humidity_rh = 51.0,
            .dewpoint_margin_c = 7.8,
            .battery_v = 3.30,
            .gate_open = 0,
            .surge_flag = 0,
            .pulse_missing = 0
        },
        {
            .scenario_name = "Foggy dawn insulator leakage",
            .pulse_peak_kv = 4.9,
            .rise_time_us = 22.0,
            .decay_time_us = 205.0,
            .pulse_interval_s = 1.01,
            .current_peak_a = 2.8,
            .ambient_temp_c = 9.0,
            .humidity_rh = 97.0,
            .dewpoint_margin_c = 0.4,
            .battery_v = 3.27,
            .gate_open = 0,
            .surge_flag = 0,
            .pulse_missing = 0
        },
        {
            .scenario_name = "West gate unlatched",
            .pulse_peak_kv = 1.2,
            .rise_time_us = 11.0,
            .decay_time_us = 54.0,
            .pulse_interval_s = 1.02,
            .current_peak_a = 0.2,
            .ambient_temp_c = 18.0,
            .humidity_rh = 62.0,
            .dewpoint_margin_c = 5.1,
            .battery_v = 3.28,
            .gate_open = 1,
            .surge_flag = 0,
            .pulse_missing = 0
        },
        {
            .scenario_name = "Remote energizer battery sag",
            .pulse_peak_kv = 3.6,
            .rise_time_us = 31.0,
            .decay_time_us = 174.0,
            .pulse_interval_s = 1.09,
            .current_peak_a = 1.7,
            .ambient_temp_c = 24.0,
            .humidity_rh = 44.0,
            .dewpoint_margin_c = 10.5,
            .battery_v = 2.92,
            .gate_open = 0,
            .surge_flag = 0,
            .pulse_missing = 0
        },
        {
            .scenario_name = "Storm front surge hit",
            .pulse_peak_kv = 9.8,
            .rise_time_us = 4.0,
            .decay_time_us = 38.0,
            .pulse_interval_s = 0.27,
            .current_peak_a = 0.7,
            .ambient_temp_c = 29.0,
            .humidity_rh = 88.0,
            .dewpoint_margin_c = 2.0,
            .battery_v = 3.24,
            .gate_open = 0,
            .surge_flag = 1,
            .pulse_missing = 0
        },
        {
            .scenario_name = "Far paddock fence dead",
            .pulse_peak_kv = 0.0,
            .rise_time_us = 0.0,
            .decay_time_us = 0.0,
            .pulse_interval_s = 4.80,
            .current_peak_a = 0.0,
            .ambient_temp_c = 17.0,
            .humidity_rh = 59.0,
            .dewpoint_margin_c = 6.8,
            .battery_v = 3.22,
            .gate_open = 0,
            .surge_flag = 0,
            .pulse_missing = 1
        }
    };

    size_t count = sizeof(demo) / sizeof(demo[0]);
    if (count > max_frames) {
        count = max_frames;
    }

    for (size_t i = 0; i < count; ++i) {
        frames[i] = demo[i];
    }

    return count;
}
