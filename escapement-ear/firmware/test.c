#include "escapement.h"
#include <assert.h>
int main(void) {
    ee_stats_t s; ee_init(&s);
    assert(!ee_edge(&s, 0xfffffff0u));
    assert(ee_edge(&s, 499984u)); /* wrap-safe 500000 us */
    assert(ee_median_interval(&s) == 500000u);
    assert(ee_rate_seconds_per_day(&s, 500000u) == 0);
    assert(!ee_edge(&s, 500000u)); /* debounce */
    assert(ee_edge(&s, 1000000u));
    assert(ee_rate_seconds_per_day(&s, 500000u) == 0);
    return 0;
}
