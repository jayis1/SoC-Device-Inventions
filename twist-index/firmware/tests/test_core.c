#include "core.h"
#include <assert.h>
int main(void) {
    assert(!valid_length_mm(9)); assert(valid_length_mm(10));
    assert(valid_length_mm(10000)); assert(!valid_length_mm(10001));
    assert(turns_per_km(100, 1000) == 100000);
    assert(turns_per_km(1, 10000) == 100);
    assert(turns_per_km(3, 0) == 0);
    return 0;
}
