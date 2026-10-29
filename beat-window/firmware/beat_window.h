#ifndef BEAT_WINDOW_H
#define BEAT_WINDOW_H
#include <stdbool.h>
#include <stdint.h>
struct beat_window { uint64_t last_us; bool seen; };
/* Returns true for every crossing after the first, with interval_us set. */
bool beat_window_crossing(struct beat_window *state, uint64_t now_us, uint64_t *interval_us);
#endif
