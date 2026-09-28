#include "filter_core.h"

#include <math.h>

static bool config_valid(const ff_config_t *c) {
    return c != NULL && isfinite(c->reference_flow_dp_pa) &&
           isfinite(c->fan_off_dp_pa) && isfinite(c->minimum_valid_flow_dp_pa) &&
           isfinite(c->warn_ratio) && isfinite(c->replace_ratio) &&
           c->reference_flow_dp_pa > 0.0f && c->fan_off_dp_pa >= 0.0f &&
           c->minimum_valid_flow_dp_pa > c->fan_off_dp_pa &&
           c->warn_ratio > 1.0f && c->replace_ratio > c->warn_ratio;
}

ff_result_t ff_evaluate(const ff_sample_t *s, const ff_config_t *c) {
    ff_result_t r = {FF_SENSOR_FAULT, NAN, NAN};
    if (s == NULL || !config_valid(c) || !s->filter_sensor_ok || !s->flow_sensor_ok ||
        !isfinite(s->filter_dp_pa) || !isfinite(s->flow_dp_pa) ||
        !isfinite(s->temperature_c) || !isfinite(s->humidity_pct)) {
        return r;
    }
    if (s->filter_dp_pa < -1.0f || s->filter_dp_pa > 500.0f ||
        s->flow_dp_pa < -1.0f || s->flow_dp_pa > 125.0f) {
        r.status = FF_TUBE_FAULT;
        return r;
    }
    if (s->flow_dp_pa <= c->fan_off_dp_pa) {
        r.status = FF_FAN_OFF;
        return r;
    }
    if (s->flow_dp_pa < c->minimum_valid_flow_dp_pa) {
        r.status = FF_LOW_FLOW;
        return r;
    }
    if (s->filter_dp_pa < 0.5f) {
        r.status = FF_TUBE_FAULT;
        return r;
    }

    r.normalized_filter_dp_pa = s->filter_dp_pa * c->reference_flow_dp_pa /
                                s->flow_dp_pa;
    if (!isfinite(c->clean_normalized_dp_pa) || c->clean_normalized_dp_pa <= 0.0f) {
        r.status = FF_LEARN_REQUIRED;
        return r;
    }
    r.loading_ratio = r.normalized_filter_dp_pa / c->clean_normalized_dp_pa;
    if (r.loading_ratio >= c->replace_ratio) {
        r.status = FF_REPLACE;
    } else if (r.loading_ratio >= c->warn_ratio) {
        r.status = FF_WARN_LOADING;
    } else {
        r.status = FF_OK;
    }
    return r;
}

bool ff_calibrate_clean(const ff_sample_t *samples, size_t count,
                        const ff_config_t *config, float *baseline_pa) {
    float values[FF_MAX_CALIBRATION_SAMPLES];
    size_t used = 0U;
    size_t i;
    if (samples == NULL || baseline_pa == NULL || !config_valid(config) ||
        count < 5U || count > FF_MAX_CALIBRATION_SAMPLES) {
        return false;
    }
    for (i = 0U; i < count; ++i) {
        const ff_sample_t *s = &samples[i];
        if (s->filter_sensor_ok && s->flow_sensor_ok && isfinite(s->filter_dp_pa) &&
            isfinite(s->flow_dp_pa) && s->filter_dp_pa >= 0.0f &&
            s->filter_dp_pa <= 500.0f &&
            s->flow_dp_pa >= config->minimum_valid_flow_dp_pa &&
            s->flow_dp_pa <= 125.0f) {
            size_t j = used;
            const float v = s->filter_dp_pa * config->reference_flow_dp_pa /
                            s->flow_dp_pa;
            while (j > 0U && values[j - 1U] > v) {
                values[j] = values[j - 1U];
                --j;
            }
            values[j] = v;
            ++used;
        }
    }
    if (used < 5U) {
        return false;
    }
    *baseline_pa = (used % 2U != 0U) ? values[used / 2U] :
                   0.5f * (values[used / 2U - 1U] + values[used / 2U]);
    return true;
}

void ff_tracker_init(ff_tracker_t *t, unsigned required_count) {
    if (t != NULL) {
        t->stable_status = FF_OK;
        t->pending_status = FF_OK;
        t->pending_count = 0U;
        t->required_count = required_count == 0U ? 1U : required_count;
    }
}

ff_status_t ff_tracker_update(ff_tracker_t *t, ff_status_t candidate) {
    if (t == NULL) {
        return FF_SENSOR_FAULT;
    }
    if (candidate != FF_WARN_LOADING && candidate != FF_REPLACE) {
        t->stable_status = candidate;
        t->pending_status = candidate;
        t->pending_count = 0U;
        return candidate;
    }
    if (candidate != t->pending_status) {
        t->pending_status = candidate;
        t->pending_count = 1U;
    } else if (t->pending_count < t->required_count) {
        ++t->pending_count;
    }
    if (t->pending_count >= t->required_count) {
        t->stable_status = candidate;
    }
    return t->stable_status;
}

const char *ff_status_name(ff_status_t status) {
    static const char *const names[] = {
        "OK", "WARN_LOADING", "REPLACE", "FAN_OFF", "LOW_FLOW",
        "LEARN_REQUIRED", "TUBE_FAULT", "SENSOR_FAULT"
    };
    const size_t count = sizeof(names) / sizeof(names[0]);
    return (status >= FF_OK && (size_t)status < count) ? names[status] : "UNKNOWN";
}
