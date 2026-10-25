#ifndef SHUTTER_TICK_TIMING_H
#define SHUTTER_TICK_TIMING_H
#include <stdbool.h>
#include <stdint.h>
typedef struct { bool waiting_for_rise; uint64_t rise_us; } shutter_capture_t;
bool shutter_capture_edge(shutter_capture_t *s, bool light_present,
                          uint64_t now_us, uint32_t *duration_us);
#endif
