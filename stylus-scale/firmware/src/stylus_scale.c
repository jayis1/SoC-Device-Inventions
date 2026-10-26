#include "stylus_scale.h"
#include <stddef.h>

void ss_filter_init(ss_filter_t *f) { f->count = 0; f->next = 0; }
void ss_filter_push(ss_filter_t *f, int32_t sample) {
    f->samples[f->next] = sample;
    f->next = (uint8_t)((f->next + 1U) % SS_FILTER_N);
    if (f->count < SS_FILTER_N) ++f->count;
}
static int32_t median(int32_t *v, uint8_t n) {
    for (uint8_t i=1; i<n; ++i) { int32_t x=v[i]; uint8_t j=i; while(j && v[j-1]>x){v[j]=v[j-1];--j;} v[j]=x; }
    return v[n/2U];
}
int32_t ss_filter_value(const ss_filter_t *f) {
    if (!f->count) return 0;
    int32_t tmp[SS_FILTER_N];
    for(uint8_t i=0;i<f->count;++i) tmp[i]=f->samples[i];
    return median(tmp,f->count);
}
float ss_grams(int32_t raw, int32_t zero, float counts_per_gram) {
    if (!(counts_per_gram > 0.0f)) return 0.0f;
    return (float)(raw-zero)/counts_per_gram;
}
