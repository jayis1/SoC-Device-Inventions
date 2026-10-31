#include <assert.h>
#include <stdint.h>
#include "../firmware/main.c"
int main(void) {
    uint32_t v = 0;
    assert(thread_gauge_tex(500, 10000, &v) && v == 50); /* 0.5 g / 10 m = 50 tex */
    assert(thread_gauge_denier(500, 10000, &v) && v == 450);
    assert(!thread_gauge_tex(0, 10, &v));
    assert(!thread_gauge_tex(1, 0, &v));
    assert(!thread_gauge_tex(1, 1, 0));
    return 0;
}
