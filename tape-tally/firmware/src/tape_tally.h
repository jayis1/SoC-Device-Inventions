#pragma once
#include <stdbool.h>
#include <stdint.h>
typedef enum { TT_NONE=0, TT_PULSE, TT_STALL } tt_event_type;
typedef struct { tt_event_type type; uint32_t value_ms; } tt_event;
bool tt_sample(bool level, uint32_t now_ms, tt_event *event);
