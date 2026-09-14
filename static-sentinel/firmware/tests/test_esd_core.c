#include "esd_core.h"
#include <assert.h>
#include <math.h>

int main(void) {
    float ohms = 0.0f;
    assert(esd_ohms_from_divider(3.3f, 1000000.0f, 1.65f, &ohms));
    assert(fabsf(ohms - 1000000.0f) < 1.0f);
    assert(!esd_ohms_from_divider(3.3f, 1.0f, 3.3f, &ohms));
    const esd_policy_t p = {7.5e5f, 1.0e9f, 7.5e5f, 3.5e7f, 30.0f};
    esd_sample_t s = {1.0e6f, 1.0e6f, 45.0f, true};
    assert(esd_evaluate(&s, &p) == ESD_OK);
    s.mat_ohms = 10.0f;
    assert(esd_evaluate(&s, &p) == ESD_FAIL_MAT);
    s.mat_ohms = 1.0e6f; s.strap_ohms = 4.0e7f;
    assert(esd_evaluate(&s, &p) == ESD_FAIL_STRAP);
    s.strap_ohms = 1.0e6f; s.relative_humidity_pct = 20.0f;
    assert(esd_evaluate(&s, &p) == ESD_WARN_HUMIDITY);
    s.interlock_closed = false;
    assert(esd_evaluate(&s, &p) == ESD_INVALID_SAMPLE);
    return 0;
}
