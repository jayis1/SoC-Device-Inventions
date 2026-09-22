#include <assert.h>
#include <string.h>
#include "warp_watch.h"

static warp_watch_config_t valid_config(void) {
    return (warp_watch_config_t){150U, 800U, 30U};
}

int main(void) {
    warp_watch_config_t config = valid_config();
    warp_watch_sample_t sample = {true, true, 425U, 0U};
    assert(warp_watch_config_valid(&config));
    assert(warp_watch_evaluate(&config, &sample).state == WARP_WATCH_STABLE);
    sample.tension_cN = 149U;
    assert(warp_watch_evaluate(&config, &sample).state == WARP_WATCH_SLACK);
    sample.tension_cN = 801U;
    assert(warp_watch_evaluate(&config, &sample).state == WARP_WATCH_OVER_TENSION);
    sample.tension_cN = 400U;
    sample.seconds_without_motion = 30U;
    assert(warp_watch_evaluate(&config, &sample).state == WARP_WATCH_TAKEUP_STALLED);
    sample.force_valid = false;
    assert(warp_watch_evaluate(&config, &sample).state == WARP_WATCH_SENSOR_FAULT);
    config.maximum_cN = 100U;
    assert(!warp_watch_config_valid(&config));
    assert(strcmp(warp_watch_state_name(WARP_WATCH_STABLE), "STABLE") == 0);
    return 0;
}
