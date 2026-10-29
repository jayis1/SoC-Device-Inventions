#include "detector.h"
bool detector_update(detector_t *d, uint16_t sample, uint32_t now_us) {
    bool event = false;
    if (!d->armed) {
        if (sample < 1800) { d->armed = true; d->state = false; }
        else if (sample > 2295) { d->armed = true; d->state = true; }
        return false;
    }
    if (!d->state && sample > 2295) {
        d->state = true;
        if ((uint32_t)(now_us - d->last_us) >= 20000u) { event = true; d->last_us = now_us; }
    } else if (d->state && sample < 1800) {
        d->state = false;
    }
    return event;
}
