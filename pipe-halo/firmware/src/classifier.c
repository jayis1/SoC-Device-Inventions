#include "classifier.h"

const char *ph_state_name(ph_state_t state)
{
    switch (state) {
    case PH_STATE_IDLE: return "IDLE";
    case PH_STATE_NORMAL_FLOW: return "NORMAL_FLOW";
    case PH_STATE_DRIP_LEAK: return "DRIP_LEAK";
    case PH_STATE_WATER_HAMMER: return "WATER_HAMMER";
    case PH_STATE_FREEZE_RISK: return "FREEZE_RISK";
    case PH_STATE_BURST_RISK: return "BURST_RISK";
    case PH_STATE_VALVE_TAMPER: return "VALVE_TAMPER";
    default: return "UNKNOWN";
    }
}

void ph_classify_window(const ph_sensor_window_t *window,
                        const ph_features_t *features,
                        ph_decision_t *decision)
{
    decision->state = PH_STATE_IDLE;
    decision->recommended_action = "No action required; continue monitoring.";

    if (features->burst_risk_score >= 0.72f) {
        decision->state = PH_STATE_BURST_RISK;
        decision->recommended_action = "Close the supply if possible and apply active freeze mitigation immediately.";
        return;
    }

    if (features->freeze_margin_c <= 0.8f && features->flow_index < 0.18f) {
        decision->state = PH_STATE_FREEZE_RISK;
        decision->recommended_action = "Raise room temperature or start controlled trickle flow to protect the pipe.";
        return;
    }

    if (features->hammer_index >= 0.52f) {
        decision->state = PH_STATE_WATER_HAMMER;
        decision->recommended_action = "Inspect arrestors, fast-closing valves, or partially closed shutoffs.";
        return;
    }

    if (features->drip_confidence >= 0.55f && features->flow_index < 0.45f) {
        decision->state = PH_STATE_DRIP_LEAK;
        decision->recommended_action = "Inspect nearby fixture or trap for periodic drip.";
        return;
    }

    if (window->valve_angle_deg < 55.0f && features->flow_index > 0.35f) {
        decision->state = PH_STATE_VALVE_TAMPER;
        decision->recommended_action = "Valve appears partially closed; verify the shutoff position and line restrictions.";
        return;
    }

    if (features->flow_index >= 0.20f) {
        decision->state = PH_STATE_NORMAL_FLOW;
        decision->recommended_action = "Water movement appears normal.";
    }
}
