#pragma once
#include <stdbool.h>
#include <stdint.h>
typedef struct { bool armed; bool state; uint32_t last_us; } detector_t;
// Return true once per qualified magnetic crossing. ADC samples are 0..4095.
bool detector_update(detector_t *d, uint16_t sample, uint32_t now_us);
