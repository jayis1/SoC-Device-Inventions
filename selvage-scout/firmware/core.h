#ifndef SELVAGE_CORE_H
#define SELVAGE_CORE_H
#include <stdint.h>
typedef struct { uint32_t events; uint32_t test; uint32_t last_ms; uint8_t active; uint8_t armed; } scout_t;
void scout_init(scout_t *s);
/* threshold in ADC counts (0..4095); high means reflective target. */
int scout_sample(scout_t *s, uint16_t adc, uint16_t threshold, uint32_t now_ms);
void scout_start(scout_t *s, uint32_t now_ms);
#endif
