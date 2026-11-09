#ifndef HINGE_CYCLE_H
#define HINGE_CYCLE_H
#include <stdbool.h>
#include <stdint.h>
#define HINGE_CYCLE_RAW_MAX 4095u
typedef struct {
    uint32_t cycle;
    uint16_t angle_raw;
    int16_t angle_tenths_deg;
    bool opening;
} hinge_cycle_event_t;
/* Emits one event on an open-to-closed or closed-to-open direction reversal.
 * Caller supplies bounded raw angle [0,4095] and a non-null output. */
bool hinge_cycle_update(uint16_t raw, hinge_cycle_event_t *event);
void hinge_cycle_reset(void);
#endif
