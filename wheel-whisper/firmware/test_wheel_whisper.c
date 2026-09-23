/* Wheel Whisper portable-core test. Designed by jayis1. */
#include "wheel_whisper.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void require(int condition, const char *message) {
    if (!condition) { fprintf(stderr, "FAIL: %s\n", message); exit(1); }
}

int main(void) {
    uint16_t tone[960], flat[960];
    for (unsigned i = 0; i < 960; ++i) {
        tone[i] = (uint16_t)(2048.0f + 400.0f * sinf(2.0f * 3.14159265358979323846f * 250.0f * (float)i / 8000.0f));
        flat[i] = 2048;
    }
    ww_measurement_t good = ww_analyze_capture(tone, 960, 8000, 80, 800, 5, 4095);
    ww_measurement_t bad = ww_analyze_capture(flat, 960, 8000, 80, 800, 5, 4095);
    require(good.status == WW_CAPTURE_OK, "tone accepted");
    require(fabsf(good.frequency_hz - 250.0f) <= 5.0f, "frequency located");
    require(bad.status == WW_CAPTURE_TOO_WEAK, "flat capture rejected");
    puts("wheel_whisper tests passed");
    return 0;
}
