#include "kernel_scout.h"
size_t ks_demo_samples(ks_sample_t *s, size_t n) {
    if (n < 3) return 0;
    s[0] = (ks_sample_t){{1018,1020,1019,1021,1019},93.8f,18.2f,20.0f,52.0f,1.0f};
    s[1] = (ks_sample_t){{1138,1140,1139,1141,1140},89.4f,26.6f,27.0f,71.0f,5.0f};
    s[2] = (ks_sample_t){{1238,1240,1239,1241,1240},91.1f,31.0f,30.5f,74.0f,13.0f};
    return 3;
}
