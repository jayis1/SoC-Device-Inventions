#include <assert.h>
#include <stdio.h>
#include "rain_scribe.c"

int main(void) {
    rain_scribe_level(1, 1);
    rain_scribe_start(100);
    rain_scribe_level(120, 0);
    rain_scribe_level(130, 0);
    rain_scribe_level(900, 5);
    rain_scribe_level(250, 2);
    rain_scribe_mark(300);
    rain_scribe_end(400);
    rain_scribe_end(500);
    assert(!active);
    puts("rain_scribe core tests passed");
    return 0;
}
