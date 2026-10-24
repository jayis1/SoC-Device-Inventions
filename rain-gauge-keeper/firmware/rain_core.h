#ifndef RAIN_CORE_H
#define RAIN_CORE_H
#include <stdint.h>
#include <stdbool.h>
typedef struct { uint32_t last_ms; bool seen; } rain_debounce_t;
bool rain_accept_tip(rain_debounce_t *state, uint32_t now_ms);
#endif
