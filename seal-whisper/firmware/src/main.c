#include <stdio.h>
#include "leak_model.h"
int main(void) {
    /* 10 s post-settle capture: a healthy test package exhibits a slow recovery. */
    float p[]={-35000,-34999.2f,-34998.5f,-34997.7f,-34996.9f,-34996.1f};
    float t[]={22.00f,22.01f,22.01f,22.02f,22.02f,22.03f};
    sw_result_t r=sw_analyze(p,t,6,2.0f,7.5f,25.0f,3.0f,3.0f,2.5f);
    printf("leak=%.2f Pa/s ringdown=%.2f ms pz=%.2f az=%.2f %s confidence=%.0f%%\n",
      r.pressure_pa_s,r.ringdown_ms,r.pressure_z,r.acoustic_z,r.pass?"PASS":"INVESTIGATE",r.confidence_pct);
    return r.pass ? 0 : 1;
}
