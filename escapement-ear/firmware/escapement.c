#include "escapement.h"
#include <stddef.h>
void ee_init(ee_stats_t *s) { if (s) { *s = (ee_stats_t){0}; } }
bool ee_edge(ee_stats_t *s, uint32_t now_us) {
    if (!s) return false;
    if (!s->have_last) { s->last_us = now_us; s->have_last = true; return false; }
    uint32_t dt = now_us - s->last_us; /* unsigned subtraction handles timer wrap */
    s->last_us = now_us;
    if (dt < EE_MIN_INTERVAL_US) return false;
    s->intervals[s->next] = dt;
    s->next = (uint8_t)((s->next + 1u) % EE_INTERVAL_CAPACITY);
    if (s->count < EE_INTERVAL_CAPACITY) s->count++;
    return true;
}
uint32_t ee_median_interval(const ee_stats_t *s) {
    if (!s || !s->count) return 0;
    uint32_t a[EE_INTERVAL_CAPACITY];
    for (uint8_t i=0; i<s->count; i++) a[i]=s->intervals[i];
    for (uint8_t i=1; i<s->count; i++) { uint32_t v=a[i]; int j=(int)i-1; while(j>=0 && a[j]>v){a[j+1]=a[j];j--;} a[j+1]=v; }
    return a[s->count/2u];
}
int32_t ee_rate_seconds_per_day(const ee_stats_t *s, uint32_t target_us) {
    uint32_t measured=ee_median_interval(s);
    if (!measured || !target_us) return 0;
    int64_t numerator=((int64_t)target_us-(int64_t)measured)*86400LL;
    return (int32_t)(numerator / measured);
}
