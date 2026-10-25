#include <assert.h>
#include <stdint.h>
#include "timing.h"
int main(void) {
    shutter_capture_t s = {.waiting_for_rise = true, .rise_us = 0};
    uint32_t width = 0;
    assert(!shutter_capture_edge(&s, false, 10, &width));
    assert(!shutter_capture_edge(&s, true, 100, &width));
    assert(!shutter_capture_edge(&s, true, 150, &width));
    assert(shutter_capture_edge(&s, false, 250, &width));
    assert(width == 150);
    assert(s.waiting_for_rise);
    assert(!shutter_capture_edge(&s, false, 300, &width));
    return 0;
}
