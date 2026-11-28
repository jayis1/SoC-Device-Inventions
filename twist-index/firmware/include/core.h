#ifndef TWIST_INDEX_CORE_H
#define TWIST_INDEX_CORE_H
#include <stdbool.h>
#include <stdint.h>
bool valid_length_mm(uint32_t length_mm);
uint32_t turns_per_km(uint32_t turns, uint32_t length_mm);
#endif
