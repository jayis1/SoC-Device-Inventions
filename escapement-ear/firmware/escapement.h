#ifndef ESCAPEMENT_EAR_H
#define ESCAPEMENT_EAR_H
#include <stdbool.h>
#include <stdint.h>
#define EE_INTERVAL_CAPACITY 64
#define EE_MIN_INTERVAL_US 80000u
#define EE_DEFAULT_TARGET_US 500000u
typedef struct { uint32_t intervals[EE_INTERVAL_CAPACITY]; uint8_t count; uint8_t next; uint32_t last_us; bool have_last; } ee_stats_t;
void ee_init(ee_stats_t *s);
bool ee_edge(ee_stats_t *s, uint32_t now_us);
uint32_t ee_median_interval(const ee_stats_t *s);
int32_t ee_rate_seconds_per_day(const ee_stats_t *s, uint32_t target_us);
#endif
