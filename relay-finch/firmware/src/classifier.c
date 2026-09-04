#include "classifier.h"

const char *rf_state_name(rf_state_t state)
{
    switch (state) {
    case RF_STATE_PASS: return "PASS";
    case RF_STATE_SLOW_PULL_IN: return "SLOW_PULL_IN";
    case RF_STATE_EXCESSIVE_BOUNCE: return "EXCESSIVE_BOUNCE";
    case RF_STATE_LOW_COIL_RESISTANCE: return "LOW_COIL_RESISTANCE";
    case RF_STATE_WEAK_RETURN_SPRING: return "WEAK_RETURN_SPRING";
    case RF_STATE_WELDED_CONTACTS: return "WELDED_CONTACTS";
    case RF_STATE_NO_ACTUATION: return "NO_ACTUATION";
    default: return "UNKNOWN";
    }
}

const char *rf_mode_name(rf_test_mode_t mode)
{
    switch (mode) {
    case RF_MODE_PULL_IN_SWEEP: return "pull_in_sweep";
    case RF_MODE_HOLD_RELEASE_SWEEP: return "hold_release_sweep";
    case RF_MODE_BOUNCE_CAPTURE: return "bounce_capture";
    case RF_MODE_LIFE_SPOT_CHECK: return "life_spot_check";
    default: return "unknown";
    }
}

void rf_classify_event(const rf_preset_t *preset,
                       const rf_features_t *features,
                       rf_decision_t *decision)
{
    decision->state = RF_STATE_PASS;
    decision->recommended_action = "log_and_return_to_service";

    if (!features->actuated) {
        decision->state = RF_STATE_NO_ACTUATION;
        decision->recommended_action = "check_coil_wiring_or_replace_dut";
        return;
    }

    if (features->coil_resistance_ohm > 1.0f &&
        features->coil_resistance_ohm < preset->min_coil_resistance_ohm) {
        decision->state = RF_STATE_LOW_COIL_RESISTANCE;
        decision->recommended_action = "suspect_shorted_turns_or_wrong_part";
        return;
    }

    if (features->pull_in_v > preset->max_pull_in_v ||
        features->operate_ms > preset->max_operate_ms ||
        features->max_travel_mm < preset->min_travel_mm) {
        decision->state = RF_STATE_SLOW_PULL_IN;
        decision->recommended_action = "clean_mechanism_or_verify_supply_margin";
        return;
    }

    if (features->bounce_ms > preset->max_bounce_ms ||
        features->chatter_edges > 2) {
        decision->state = RF_STATE_EXCESSIVE_BOUNCE;
        decision->recommended_action = "inspect_contacts_for_pitting_or_alignment";
        return;
    }

    if (!features->contact_released || features->drop_out_v < 0.1f) {
        decision->state = RF_STATE_WELDED_CONTACTS;
        decision->recommended_action = "remove_from_service_immediately";
        return;
    }

    if (features->drop_out_v < preset->min_drop_out_v ||
        features->drop_out_v > preset->max_drop_out_v ||
        features->release_ms > preset->release_timeout_ms) {
        decision->state = RF_STATE_WEAK_RETURN_SPRING;
        decision->recommended_action = "inspect_return_spring_and_residual_magnetism";
        return;
    }
}
