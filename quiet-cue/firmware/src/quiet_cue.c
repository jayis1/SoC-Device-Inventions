#include "quiet_cue.h"

uint16_t qc_clamp_minutes(uint32_t minutes) {
    if (minutes < QC_MINUTES_MIN) return QC_MINUTES_MIN;
    if (minutes > QC_MINUTES_MAX) return QC_MINUTES_MAX;
    return (uint16_t)minutes;
}

uint8_t qc_led_count(uint32_t elapsed_seconds, uint16_t duration_minutes) {
    uint32_t total = (uint32_t)qc_clamp_minutes(duration_minutes) * 60u;
    if (elapsed_seconds >= total) return 0u;
    /* Ceiling division: at least one segment remains until completion. */
    return (uint8_t)(((total - elapsed_seconds) * 10u + total - 1u) / total);
}
