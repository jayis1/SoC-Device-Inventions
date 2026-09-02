#include "classifier.h"

const char *ms_state_name(ms_state_t state)
{
    switch (state) {
    case MS_STATE_NORMAL:
        return "NORMAL";
    case MS_STATE_CONDENSATION_RISK:
        return "CONDENSATION_RISK";
    case MS_STATE_HIDDEN_MOISTURE:
        return "HIDDEN_MOISTURE";
    case MS_STATE_ACTIVE_MOLD_RISK:
        return "ACTIVE_MOLD_RISK";
    case MS_STATE_WET_BUILDING_MATERIAL:
        return "WET_BUILDING_MATERIAL";
    default:
        return "UNKNOWN";
    }
}

void ms_classify_window(const ms_sensor_window_t *window,
                        const ms_features_t *features,
                        ms_decision_t *decision)
{
    (void)window;

    if (features->moisture_proxy > 0.82f && features->condensation_risk > 0.55f) {
        decision->state = MS_STATE_WET_BUILDING_MATERIAL;
        decision->recommended_action = "Probable active wetting; open the cavity or deploy drying equipment immediately.";
    } else if (features->mold_risk_score > 0.72f) {
        decision->state = MS_STATE_ACTIVE_MOLD_RISK;
        decision->recommended_action = "High mold-supporting conditions; document, dry, and plan further inspection.";
    } else if (features->moisture_proxy > 0.58f) {
        decision->state = MS_STATE_HIDDEN_MOISTURE;
        decision->recommended_action = "Elevated dielectric reading; compare against a control area and inspect nearby plumbing or flashing.";
    } else if (features->condensation_risk > 0.58f) {
        decision->state = MS_STATE_CONDENSATION_RISK;
        decision->recommended_action = "Cold surface near dew point; improve airflow, insulation, or dehumidification.";
    } else {
        decision->state = MS_STATE_NORMAL;
        decision->recommended_action = "No major anomaly relative to baseline.";
    }
}
