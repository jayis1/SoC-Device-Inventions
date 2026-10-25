#ifndef QUIET_CUE_H
#define QUIET_CUE_H
#include <stdint.h>
#define QC_MINUTES_MIN 1u
#define QC_MINUTES_MAX 120u
uint16_t qc_clamp_minutes(uint32_t minutes);
uint8_t qc_led_count(uint32_t elapsed_seconds, uint16_t duration_minutes);
#endif
