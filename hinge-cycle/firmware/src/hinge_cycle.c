#include "hinge_cycle.h"
static uint16_t previous;
static int8_t direction;
static uint32_t cycles;
static bool initialized;
void hinge_cycle_reset(void) { previous = 0; direction = 0; cycles = 0; initialized = false; }
bool hinge_cycle_update(uint16_t raw, hinge_cycle_event_t *event)
{
    if (event == 0 || raw > HINGE_CYCLE_RAW_MAX) return false;
    if (!initialized) { previous = raw; initialized = true; return false; }
    int32_t delta = (int32_t)raw - (int32_t)previous;
    previous = raw;
    if (delta > 8) {
        if (direction == -1) { ++cycles; event->cycle = cycles; event->angle_raw = raw; event->angle_tenths_deg = (int16_t)((uint32_t)raw * 3600u / 4096u); event->opening = true; direction = 1; return true; }
        direction = 1;
    } else if (delta < -8) {
        if (direction == 1) { event->cycle = cycles; event->angle_raw = raw; event->angle_tenths_deg = (int16_t)((uint32_t)raw * 3600u / 4096u); event->opening = false; direction = -1; return true; }
        direction = -1;
    }
    return false;
}
