#include "hinge_sag.h"
#include <math.h>
void hs_capture_init(hs_capture_t *c) { if (c) { c->first_mdeg=0; c->min_mdeg=0; c->max_mdeg=0; c->count=0; } }
int32_t hs_angle_mdeg(float x, float y, float z) {
    const float norm2=x*x+y*y+z*z;
    if (!isfinite(norm2) || norm2 < 0.25f) return 0;
    float a=atan2f(x, sqrtf(y*y+z*z)) * 57295.7795f;
    return (int32_t)lroundf(a);
}
int hs_capture_add(hs_capture_t *c, int32_t a, uint32_t cap) {
    if (!c || cap==0 || c->count>=cap) return 0;
    if (c->count==0) c->first_mdeg=c->min_mdeg=c->max_mdeg=a;
    if (a<c->min_mdeg) c->min_mdeg=a;
    if (a>c->max_mdeg) c->max_mdeg=a;
    c->count++;
    return 1;
}
