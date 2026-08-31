#include <math.h>

#include "classifier.h"

static float clampf(float value, float lo, float hi)
{
    if (value < lo) {
        return lo;
    }
    if (value > hi) {
        return hi;
    }
    return value;
}

const char *ss_state_name(ss_state_t state)
{
    switch (state) {
    case SS_STATE_HEALTHY:
        return "HEALTHY";
    case SS_STATE_NO_START:
        return "NO_START";
    case SS_STATE_BACKFLOW:
        return "BACKFLOW";
    case SS_STATE_SHORT_CYCLING:
        return "SHORT_CYCLING";
    case SS_STATE_DRY_RUN:
        return "DRY_RUN";
    case SS_STATE_SLOW_PUMP:
        return "SLOW_PUMP";
    case SS_STATE_LEAK_CONFIRMED:
        return "LEAK_CONFIRMED";
    case SS_STATE_HUMIDITY_ALERT:
        return "HUMIDITY_ALERT";
    default:
        return "UNKNOWN";
    }
}

void ss_classify_frame(const ss_frame_t *frame, ss_result_t *result)
{
    const bool pump_running = frame->pump_current_a > 1.0f || frame->vibration_g_rms > 0.08f;
    const bool high_water = frame->level_mm > 500.0f;
    const bool strong_backflow = frame->rebound_mm_60s > 70.0f;
    const bool shallow_rebound = frame->rebound_mm_60s > 40.0f;
    const bool slow_drawdown = frame->drawdown_rate_mm_min > -40.0f;
    const bool dry_pit = frame->level_mm < 140.0f;
    const bool humid = frame->humidity_rh > 82.0f;

    result->flood_risk_score = clampf((frame->level_mm - 280.0f) / 3.0f, 0.0f, 100.0f);
    if (frame->leak_active) {
        result->flood_risk_score = 100.0f;
    }
    if (frame->power_fail && frame->battery_v < 3.85f) {
        result->flood_risk_score = clampf(result->flood_risk_score + 12.0f, 0.0f, 100.0f);
    }

    result->cycle_health_score = 100.0f;
    if (frame->float_active && !pump_running) {
        result->cycle_health_score -= 75.0f;
    }
    if (slow_drawdown) {
        result->cycle_health_score -= 35.0f;
    }
    if (strong_backflow || shallow_rebound) {
        result->cycle_health_score -= 25.0f;
    }
    if (dry_pit && pump_running) {
        result->cycle_health_score -= 45.0f;
    }
    result->cycle_health_score = clampf(result->cycle_health_score, 0.0f, 100.0f);

    if (frame->leak_active) {
        result->state = SS_STATE_LEAK_CONFIRMED;
        result->fault_score = 100.0f;
        return;
    }

    if (frame->float_active && !pump_running && high_water) {
        result->state = SS_STATE_NO_START;
        result->fault_score = 96.0f;
        return;
    }

    if (dry_pit && pump_running && frame->drawdown_rate_mm_min > -12.0f) {
        result->state = SS_STATE_DRY_RUN;
        result->fault_score = 82.0f;
        return;
    }

    if (strong_backflow) {
        result->state = SS_STATE_BACKFLOW;
        result->fault_score = 78.0f;
        return;
    }

    if (shallow_rebound && frame->float_active) {
        result->state = SS_STATE_SHORT_CYCLING;
        result->fault_score = 70.0f;
        return;
    }

    if (pump_running && slow_drawdown && frame->level_mm > 420.0f) {
        result->state = SS_STATE_SLOW_PUMP;
        result->fault_score = 74.0f;
        return;
    }

    if (humid && !pump_running && frame->rise_rate_mm_min < 2.0f) {
        result->state = SS_STATE_HUMIDITY_ALERT;
        result->fault_score = 44.0f;
        return;
    }

    result->state = SS_STATE_HEALTHY;
    result->fault_score = clampf(100.0f - result->cycle_health_score, 0.0f, 40.0f);
}
