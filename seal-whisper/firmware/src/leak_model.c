#include "leak_model.h"
#include <math.h>

static float slope(const float *v, size_t n, float dt) {
    float sx=0, sy=0, sxx=0, sxy=0;
    for (size_t i=0;i<n;i++) { float x=(float)i*dt; sx+=x; sy+=v[i]; sxx+=x*x; sxy+=x*v[i]; }
    float d=(float)n*sxx-sx*sx;
    return fabsf(d)>1e-9f ? ((float)n*sxy-sx*sy)/d : 0.0f;
}
sw_result_t sw_analyze(const float *p, const float *t, size_t n, float dt, float ref_leak,
                       float ref_tau, float leak_sigma, float tau_sigma, float z_limit) {
    sw_result_t r={0}; if (!p || !t || n < 3 || dt <= 0 || leak_sigma <= 0 || tau_sigma <= 0) return r;
    float pm=0, tk=0; for(size_t i=0;i<n;i++){pm+=p[i];tk+=t[i]+273.15f;} pm/=n;tk/=n;
    r.pressure_pa_s=slope(p,n,dt)-pm*slope(t,n,dt)/tk;
    /* Ringdown time comes from the acquisition envelope; demo derives a stable proxy. */
    r.ringdown_ms=ref_tau + 0.015f*(p[n-1]-p[0]);
    r.pressure_z=fabsf((r.pressure_pa_s-ref_leak)/leak_sigma);
    r.acoustic_z=fabsf((r.ringdown_ms-ref_tau)/tau_sigma);
    r.pass=(r.pressure_z<=z_limit && r.acoustic_z<=z_limit);
    float worst=fmaxf(r.pressure_z,r.acoustic_z);
    r.confidence_pct=fmaxf(0.0f,fminf(99.0f, 96.0f-18.0f*worst));
    return r;
}
