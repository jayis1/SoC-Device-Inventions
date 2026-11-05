#ifndef TIDE_TAB_H
#define TIDE_TAB_H
#include <stdbool.h>
#include <stdint.h>
typedef void (*event_fn)(uint32_t ms, bool low, void *ctx);
void tide_tab_step(bool raw_low, uint32_t now_ms, bool *stable_low,
                   bool *initialized, bool *candidate, uint32_t *changed_ms,
                   event_fn emit, void *ctx);
#endif
