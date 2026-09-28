#include "filter_core.h"

#include <assert.h>
#include <math.h>

static ff_config_t config(float baseline) {
    const ff_config_t c = {25.0f, baseline, 0.8f, 4.0f, 1.8f, 2.4f};
    return c;
}

static ff_sample_t sample(float filter_pa, float flow_pa) {
    const ff_sample_t s = {filter_pa, flow_pa, 22.0f, 45.0f, true, true};
    return s;
}

int main(void) {
    ff_config_t c = config(50.0f);
    ff_sample_t s = sample(50.0f, 25.0f);
    ff_result_t r = ff_evaluate(&s, &c);
    assert(r.status == FF_OK);
    assert(fabsf(r.normalized_filter_dp_pa - 50.0f) < 0.001f);
    assert(fabsf(r.loading_ratio - 1.0f) < 0.001f);

    s = sample(100.0f, 25.0f);
    assert(ff_evaluate(&s, &c).status == FF_WARN_LOADING);
    s = sample(130.0f, 25.0f);
    assert(ff_evaluate(&s, &c).status == FF_REPLACE);
    s = sample(10.0f, 0.5f);
    assert(ff_evaluate(&s, &c).status == FF_FAN_OFF);
    s = sample(10.0f, 2.0f);
    assert(ff_evaluate(&s, &c).status == FF_LOW_FLOW);
    s = sample(-2.0f, 25.0f);
    assert(ff_evaluate(&s, &c).status == FF_TUBE_FAULT);
    s = sample(0.1f, 25.0f);
    assert(ff_evaluate(&s, &c).status == FF_TUBE_FAULT);
    s = sample(50.0f, 25.0f);
    s.flow_sensor_ok = false;
    assert(ff_evaluate(&s, &c).status == FF_SENSOR_FAULT);

    c = config(0.0f);
    s = sample(50.0f, 25.0f);
    assert(ff_evaluate(&s, &c).status == FF_LEARN_REQUIRED);

    {
        ff_sample_t clean[7] = {
            sample(48.0f, 25.0f), sample(50.0f, 25.0f), sample(49.0f, 25.0f),
            sample(200.0f, 2.0f), sample(52.0f, 25.0f), sample(51.0f, 25.0f),
            sample(80.0f, 40.0f)
        };
        float baseline = 0.0f;
        c = config(1.0f);
        assert(ff_calibrate_clean(clean, 7U, &c, &baseline));
        assert(fabsf(baseline - 50.0f) < 0.001f);
    }

    {
        ff_tracker_t tracker;
        ff_tracker_init(&tracker, 3U);
        assert(ff_tracker_update(&tracker, FF_WARN_LOADING) == FF_OK);
        assert(ff_tracker_update(&tracker, FF_WARN_LOADING) == FF_OK);
        assert(ff_tracker_update(&tracker, FF_WARN_LOADING) == FF_WARN_LOADING);
        assert(ff_tracker_update(&tracker, FF_SENSOR_FAULT) == FF_SENSOR_FAULT);
    }
    return 0;
}
