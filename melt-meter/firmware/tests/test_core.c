#include "melt_meter.h"
#include <assert.h>
int main(void) {
    int16_t q = 0; uint8_t f = 0;
    assert(mm_decode_frame((uint32_t)100 << 18, &q, &f) && q == 100 && f == 0);
    assert(mm_decode_frame((uint32_t)0x3FF0 << 18, &q, &f) && q == -16);
    assert(!mm_decode_frame(((uint32_t)1 << 16) | 3U, &q, &f) && f == 3);
    mm_state_t s; mm_record_t r; mm_init(&s);
    assert(mm_sample(&s, (uint32_t)40 << 18, 1000, &r) && r.elapsed_ms == 0 && r.valid);
    assert(!mm_sample(&s, (uint32_t)40 << 18, 1249, &r));
    assert(mm_sample(&s, (uint32_t)40 << 18, 1250, &r) && r.elapsed_ms == 250);
    return 0;
}
