#include <assert.h>
#include "fret_meter.h"
int main(void) {
    fm_state s; uint16_t raw; float mm;
    fm_init(&s, 1.0f, 3.0f);
    assert(!fm_reading(&s, &raw, &mm));
    fm_push(&s, 2048);
    assert(fm_reading(&s, &raw, &mm));
    assert(raw == 2048 && mm > 1.99f && mm < 2.01f);
    fm_init(&s, 1.0f, 1.0f); fm_push(&s, 2);
    assert(!fm_reading(&s, &raw, &mm));
    return 0;
}
