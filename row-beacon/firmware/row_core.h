#ifndef ROW_CORE_H
#define ROW_CORE_H
#include <stdbool.h>
#include <stdint.h>
typedef struct { uint32_t count; uint32_t debounce_ms; uint32_t last_event_ms; bool have_event; bool last_level; } row_core_t;
void row_core_init(row_core_t *core, uint32_t debounce_ms);
bool row_core_sample(row_core_t *core, bool hall_level, uint32_t now_ms);
bool row_core_undo(row_core_t *core);
#endif
