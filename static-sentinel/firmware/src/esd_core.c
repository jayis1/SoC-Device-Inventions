#include "esd_core.h"
#include <math.h>
#include <stddef.h>

bool esd_ohms_from_divider(float v_supply, float r_reference_ohms, float v_node, float *ohms) {
    if (ohms == NULL || !isfinite(v_supply) || !isfinite(r_reference_ohms) || !isfinite(v_node) ||
        v_supply <= 0.0f || r_reference_ohms <= 0.0f || v_node < 0.0f || v_node >= v_supply) {
        return false;
    }
    *ohms = r_reference_ohms * v_node / (v_supply - v_node);
    return isfinite(*ohms) && *ohms >= 0.0f;
}

esd_status_t esd_evaluate(const esd_sample_t *s, const esd_policy_t *p) {
    if (s == NULL || p == NULL || !s->interlock_closed || !isfinite(s->mat_ohms) ||
        !isfinite(s->strap_ohms) || !isfinite(s->relative_humidity_pct) || s->mat_ohms < 0.0f ||
        s->strap_ohms < 0.0f || s->relative_humidity_pct < 0.0f || s->relative_humidity_pct > 100.0f) {
        return ESD_INVALID_SAMPLE;
    }
    if (s->mat_ohms < p->mat_min_ohms || s->mat_ohms > p->mat_max_ohms) return ESD_FAIL_MAT;
    if (s->strap_ohms < p->strap_min_ohms || s->strap_ohms > p->strap_max_ohms) return ESD_FAIL_STRAP;
    if (s->relative_humidity_pct < p->humidity_min_pct) return ESD_WARN_HUMIDITY;
    return ESD_OK;
}

const char *esd_status_name(esd_status_t status) {
    switch (status) {
        case ESD_OK: return "OK";
        case ESD_WARN_HUMIDITY: return "WARN_HUMIDITY";
        case ESD_FAIL_MAT: return "FAIL_MAT";
        case ESD_FAIL_STRAP: return "FAIL_STRAP";
        default: return "INVALID_SAMPLE";
    }
}
