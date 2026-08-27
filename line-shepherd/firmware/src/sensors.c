#include <stddef.h>

#include "sensors.h"

size_t ls_load_demo_captures(ls_capture_t *captures, size_t max_count)
{
    static const ls_capture_t demo[] = {
        {
            .asset_id = "IRR-VALVE-07",
            .pair_name = "PAIR_B",
            .cable_type = "IRRIGATION_2C",
            .velocity_factor = 0.52f,
            .round_trip_ns = 496.0f,
            .secondary_ns = 0.0f,
            .refl_peak_v = 0.84f,
            .refl_trough_v = -0.05f,
            .ringing_ratio = 0.14f,
            .loop_res_ohm = 67.0f,
            .cap_nf = 3.9f,
            .pair_balance_pct = 96.0f,
            .foreign_voltage_v = 0.2f,
            .remote_id = 7
        },
        {
            .asset_id = "WAREHOUSE-DROP12",
            .pair_name = "PAIR_A",
            .cable_type = "CAT5E",
            .velocity_factor = 0.68f,
            .round_trip_ns = 268.0f,
            .secondary_ns = 0.0f,
            .refl_peak_v = 0.24f,
            .refl_trough_v = -0.06f,
            .ringing_ratio = 0.11f,
            .loop_res_ohm = 18.4f,
            .cap_nf = 5.4f,
            .pair_balance_pct = 71.0f,
            .foreign_voltage_v = 0.0f,
            .remote_id = 12
        },
        {
            .asset_id = "GATE-LOOP-WEST",
            .pair_name = "PAIR_C",
            .cable_type = "ALARM_4C",
            .velocity_factor = 0.57f,
            .round_trip_ns = 719.0f,
            .secondary_ns = 0.0f,
            .refl_peak_v = 0.08f,
            .refl_trough_v = -0.49f,
            .ringing_ratio = 0.41f,
            .loop_res_ohm = 92.0f,
            .cap_nf = 8.7f,
            .pair_balance_pct = 62.0f,
            .foreign_voltage_v = 0.0f,
            .remote_id = 3
        },
        {
            .asset_id = "OFFICE-BRANCH-03",
            .pair_name = "PAIR_D",
            .cable_type = "CAT6",
            .velocity_factor = 0.69f,
            .round_trip_ns = 178.0f,
            .secondary_ns = 226.0f,
            .refl_peak_v = 0.53f,
            .refl_trough_v = -0.04f,
            .ringing_ratio = 0.22f,
            .loop_res_ohm = 15.2f,
            .cap_nf = 3.6f,
            .pair_balance_pct = 88.0f,
            .foreign_voltage_v = 0.1f,
            .remote_id = 0
        },
        {
            .asset_id = "PUMP-HOUSE-SENS1",
            .pair_name = "PAIR_A",
            .cable_type = "ALARM_2C",
            .velocity_factor = 0.63f,
            .round_trip_ns = 128.0f,
            .secondary_ns = 0.0f,
            .refl_peak_v = 0.31f,
            .refl_trough_v = -0.10f,
            .ringing_ratio = 0.18f,
            .loop_res_ohm = 46.8f,
            .cap_nf = 2.4f,
            .pair_balance_pct = 91.0f,
            .foreign_voltage_v = 0.0f,
            .remote_id = 1
        },
        {
            .asset_id = "CONF-RM-04",
            .pair_name = "PAIR_B",
            .cable_type = "CAT5E",
            .velocity_factor = 0.68f,
            .round_trip_ns = 302.0f,
            .secondary_ns = 0.0f,
            .refl_peak_v = 0.04f,
            .refl_trough_v = -0.03f,
            .ringing_ratio = 0.05f,
            .loop_res_ohm = 16.9f,
            .cap_nf = 4.2f,
            .pair_balance_pct = 98.0f,
            .foreign_voltage_v = 0.0f,
            .remote_id = 4
        },
        {
            .asset_id = "PARK-LIGHT-LOOP",
            .pair_name = "PAIR_A",
            .cable_type = "FENCE_WIRE",
            .velocity_factor = 0.80f,
            .round_trip_ns = 0.0f,
            .secondary_ns = 0.0f,
            .refl_peak_v = 0.00f,
            .refl_trough_v = 0.00f,
            .ringing_ratio = 0.00f,
            .loop_res_ohm = 0.0f,
            .cap_nf = 0.0f,
            .pair_balance_pct = 0.0f,
            .foreign_voltage_v = 23.5f,
            .remote_id = 0
        }
    };

    size_t count = sizeof(demo) / sizeof(demo[0]);
    if (count > max_count) {
        count = max_count;
    }

    for (size_t i = 0; i < count; ++i) {
        captures[i] = demo[i];
    }
    return count;
}
