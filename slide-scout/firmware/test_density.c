#include <assert.h>
#include <math.h>
#include <stddef.h>
#include "density.h"
int main(void) {
    double od = -1.0;
    assert(density_from_ratio(1000.0, 100.0, &od));
    assert(fabs(od - 1.0) < 1e-12);
    assert(!density_from_ratio(100.0, 0.0, &od));
    assert(!density_from_ratio(100.0, 200.0, &od));
    assert(!density_from_ratio(NAN, 1.0, &od));
    assert(!density_from_ratio(100.0, 1.0, NULL));
    return 0;
}
