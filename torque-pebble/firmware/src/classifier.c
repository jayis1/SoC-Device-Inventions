#include "classifier.h"

void tp_classify_event(const tp_event_t *event,
                       const tp_features_t *features,
                       tp_decision_t *decision)
{
    decision->state = TP_STATE_PASS;
    decision->recommended_action = "Accept joint";

    if (features->backoff_deg > 1.5f) {
        decision->state = TP_STATE_REWORK_DETECTED;
        decision->recommended_action = "Inspect joint and repeat tightening";
        return;
    }

    if (features->cross_thread_score >= 0.62f) {
        decision->state = TP_STATE_CROSS_THREAD_RISK;
        decision->recommended_action = "Back out fastener and inspect threads";
        return;
    }

    if (features->peak_torque_nm < event->preset->torque_low_nm) {
        decision->state = TP_STATE_UNDER_TORQUE;
        decision->recommended_action = "Retighten to target torque";
        return;
    }

    if (features->peak_torque_nm > event->preset->torque_high_nm) {
        decision->state = TP_STATE_OVER_TORQUE;
        decision->recommended_action = "Check for fastener damage or yielding";
        return;
    }

    if (event->preset->enforce_angle) {
        float angle_low = event->preset->target_angle_deg - event->preset->angle_window_deg;
        float angle_high = event->preset->target_angle_deg + event->preset->angle_window_deg;
        if (features->angle_after_snug_deg < angle_low) {
            decision->state = TP_STATE_UNDER_TORQUE;
            decision->recommended_action = "Increase angle after snug";
            return;
        }
        if (features->angle_after_snug_deg > angle_high || features->yield_risk > 0.22f) {
            decision->state = TP_STATE_SOFT_JOINT;
            decision->recommended_action = "Inspect joint stack and compression";
            return;
        }
    }
}
