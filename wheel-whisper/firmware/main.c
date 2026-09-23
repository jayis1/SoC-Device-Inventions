/* Wheel Whisper host demonstration adapter. Designed by jayis1.
 * Reference-design code: host-tested; target SDK integration is not validated.
 */
#include "wheel_whisper.h"
#include <math.h>
#include <stdio.h>

int main(void) {
    uint16_t samples[960];
    for (unsigned i = 0; i < 960; ++i) {
        float v = 2048.0f + 550.0f * sinf(2.0f * 3.14159265358979323846f * 320.0f * (float)i / 8000.0f);
        samples[i] = (uint16_t)v;
    }
    ww_measurement_t m = ww_analyze_capture(samples, 960, 8000, 80, 1200, 5, 4095);
    printf("frequency=%.1f Hz quality=%.2f status=%d\n", m.frequency_hz, m.quality, m.status);
    return m.status == WW_CAPTURE_OK ? 0 : 1;
}
