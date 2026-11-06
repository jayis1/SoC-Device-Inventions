#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint64_t ticks;
    uint32_t edges_per_revolution;
    uint32_t circumference_mm;
    uint32_t events;
    uint64_t previous_event_mm;
} row_meter_t;

void row_meter_init(row_meter_t *m, uint32_t edges_per_revolution, uint32_t circumference_mm);
void row_meter_edge(row_meter_t *m);
uint64_t row_meter_distance_mm(const row_meter_t *m);
bool row_meter_mark(row_meter_t *m, uint64_t *spacing_mm);
