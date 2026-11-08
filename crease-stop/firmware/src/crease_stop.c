#include "crease_stop.h"

bool crease_stop_record(bool pressed, uint16_t adc_raw,
                        crease_stop_event_t *event)
{
    static bool was_pressed;
    static uint32_t sequence;
    if (event == 0 || adc_raw > CREASE_STOP_ADC_MAX) {
        return false;
    }
    if (!pressed) {
        was_pressed = false;
        return false;
    }
    if (was_pressed) {
        return false;
    }
    was_pressed = true;
    event->sequence = ++sequence;
    event->position_raw = adc_raw;
    return true;
}
