/* Host demonstration adapter. Target RP2040 adapter is board-specific. */
#include <stdio.h>
#include "warp_watch.h"

int main(void) {
    const warp_watch_config_t config = {150U, 800U, 30U};
    const warp_watch_sample_t sample = {true, true, 425U, 2U};
    const warp_watch_output_t output = warp_watch_evaluate(&config, &sample);
    printf("state=%s amber=%u red=%u beep_ms=%u\n", warp_watch_state_name(output.state),
           output.amber_led, output.red_led, output.beep_ms);
    return output.state == WARP_WATCH_STABLE ? 0 : 1;
}
