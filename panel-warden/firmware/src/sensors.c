#include "sensors.h"

size_t pw_load_demo_frames(pw_frame_t *frames, size_t max_frames)
{
    static const pw_frame_t demo[] = {
        {
            .asset_id = "MCC-A1",
            .hotspot_c = 87.2f,
            .background_c = 44.9f,
            .hotspot_area_px = 9.0f,
            .rise_rate_c_per_hr = 5.6f,
            .ultrasound_db = 18.0f,
            .rf_dbm_equiv = -53.0f,
            .burst_rate_hz = 1.6f,
            .load_corr_a = 36.0f,
            .humidity_pct = 42.0f,
            .ambient_c = 35.0f,
            .contact_c = 40.5f,
            .ct_present = true
        },
        {
            .asset_id = "PV-COMB-2",
            .hotspot_c = 53.1f,
            .background_c = 40.7f,
            .hotspot_area_px = 6.0f,
            .rise_rate_c_per_hr = 2.4f,
            .ultrasound_db = 37.5f,
            .rf_dbm_equiv = -45.0f,
            .burst_rate_hz = 5.9f,
            .load_corr_a = 11.0f,
            .humidity_pct = 94.0f,
            .ambient_c = 29.0f,
            .contact_c = 28.8f,
            .ct_present = true
        },
        {
            .asset_id = "CHILLER-BKT3",
            .hotspot_c = 49.8f,
            .background_c = 43.5f,
            .hotspot_area_px = 5.0f,
            .rise_rate_c_per_hr = 0.8f,
            .ultrasound_db = 33.2f,
            .rf_dbm_equiv = -59.0f,
            .burst_rate_hz = 3.4f,
            .load_corr_a = 8.5f,
            .humidity_pct = 51.0f,
            .ambient_c = 31.2f,
            .contact_c = 34.0f,
            .ct_present = true
        },
        {
            .asset_id = "WELL-PANEL-7",
            .hotspot_c = 62.0f,
            .background_c = 31.0f,
            .hotspot_area_px = 4.0f,
            .rise_rate_c_per_hr = 6.9f,
            .ultrasound_db = 44.0f,
            .rf_dbm_equiv = -34.0f,
            .burst_rate_hz = 11.5f,
            .load_corr_a = 14.0f,
            .humidity_pct = 58.0f,
            .ambient_c = 26.0f,
            .contact_c = 29.0f,
            .ct_present = true
        },
        {
            .asset_id = "BREAKER-L2",
            .hotspot_c = 75.0f,
            .background_c = 43.0f,
            .hotspot_area_px = 20.0f,
            .rise_rate_c_per_hr = 4.0f,
            .ultrasound_db = 11.0f,
            .rf_dbm_equiv = -63.0f,
            .burst_rate_hz = 0.7f,
            .load_corr_a = 71.0f,
            .humidity_pct = 39.0f,
            .ambient_c = 33.0f,
            .contact_c = 39.0f,
            .ct_present = true
        },
        {
            .asset_id = "PUMP-HOUSE-1",
            .hotspot_c = 30.7f,
            .background_c = 29.6f,
            .hotspot_area_px = 2.0f,
            .rise_rate_c_per_hr = 0.2f,
            .ultrasound_db = 9.0f,
            .rf_dbm_equiv = -68.0f,
            .burst_rate_hz = 0.1f,
            .load_corr_a = 0.0f,
            .humidity_pct = 98.0f,
            .ambient_c = 26.0f,
            .contact_c = 26.1f,
            .ct_present = false
        }
    };

    const size_t available = sizeof(demo) / sizeof(demo[0]);
    const size_t count = (max_frames < available) ? max_frames : available;

    for (size_t i = 0; i < count; ++i) {
        frames[i] = demo[i];
    }

    return count;
}
