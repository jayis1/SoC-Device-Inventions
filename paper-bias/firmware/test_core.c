#include "paper_bias.h"
#include <assert.h>
#include <stdint.h>

int main(void) {
    pb_accumulator_t acc;
    uint16_t mean = 0;
    uint32_t ratio = 0;
    pb_reset(&acc);
    assert(pb_add(&acc, 1000, &mean) && mean == 1000);
    assert(pb_add(&acc, 2000, &mean) && mean == 1500);
    assert(!pb_add(&acc, 4096, &mean));
    assert(pb_directional_ratio(1800, 1500, 16, &ratio) && ratio == 1200);
    assert(!pb_directional_ratio(0, 1500, 16, &ratio));
    return 0;
}
