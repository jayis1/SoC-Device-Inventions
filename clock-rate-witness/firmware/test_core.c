#include "clock_rate.h"
#include <assert.h>
int main(void) {
    cr_state_t s; uint64_t interval = 99;
    cr_init(&s);
    assert(cr_edge(&s, 1000, &interval) == 0 && interval == 0);
    assert(cr_edge(&s, 10001000, &interval) == 1 && interval == 10000000);
    assert(cr_error_ppm(10001000, 10000000) == 100);
    assert(cr_edge(&s, 9000, &interval) == -2);
    assert(s.count == 1);
    assert(cr_edge(0, 1, &interval) == -1);
    return 0;
}
