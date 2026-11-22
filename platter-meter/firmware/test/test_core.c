#include <assert.h>
#include <math.h>
#include "../src/platter_meter.h"
int main(void) {
    unsigned steady[8] = {1800,1800,1800,1800,1800,1800,1800,1800};
    result_t r;
    assert(analyze_intervals(steady, 8, &r));
    assert(fabs(r.rpm - 33.3333333) < 0.001);
    assert(r.interval_cv_pct == 0.0 && r.samples == 8);
    unsigned invalid[8] = {1800,1800,1800,1800,1800,1800,1800,7500};
    assert(!analyze_intervals(invalid, 8, &r));
    assert(!analyze_intervals(steady, 7, &r));
    assert(!analyze_intervals(NULL, 8, &r));
    return 0;
}
