#include "row_meter.h"
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
int main(void) {
    row_meter_t m; uint64_t spacing;
    row_meter_init(&m, 80, 1000);
    assert(row_meter_distance_mm(&m) == 0);
    for (int i=0;i<40;i++) row_meter_edge(&m);
    assert(row_meter_distance_mm(&m) == 500);
    assert(row_meter_mark(&m, &spacing) && spacing == 500);
    for (int i=0;i<80;i++) row_meter_edge(&m);
    assert(row_meter_mark(&m, &spacing) && spacing == 1000);
    row_meter_init(&m, 0, 1000);
    assert(row_meter_distance_mm(&m) == 0);
    assert(!row_meter_mark(&m, NULL));
    row_meter_init(&m, 1, UINT32_MAX);
    m.ticks = UINT64_MAX;
    assert(row_meter_distance_mm(&m) == UINT64_MAX);
    row_meter_edge(&m);
    assert(m.ticks == UINT64_MAX);
    return 0;
}
