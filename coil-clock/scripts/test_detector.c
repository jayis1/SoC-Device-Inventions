/* Minimal SDK-independent detector exerciser: gcc -std=c11 -Wall -Wextra -Werror -I firmware firmware/detector.c scripts/test_detector.c -o /tmp/test-coil-clock && /tmp/test-coil-clock */
#include <assert.h>
#include "detector.h"
int main(void) {
    detector_t d = {0};
    assert(!detector_update(&d, 1000, 1000));
    assert(detector_update(&d, 2500, 30000));
    assert(!detector_update(&d, 1000, 40000));
    assert(detector_update(&d, 2500, 60000));
    assert(!detector_update(&d, 2500, 70000));
    return 0;
}
