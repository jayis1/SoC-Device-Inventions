#include "core.h"
bool valid_length_mm(uint32_t length_mm) { return length_mm >= 10U && length_mm <= 10000U; }
uint32_t turns_per_km(uint32_t turns, uint32_t length_mm) {
    if (length_mm == 0U) return 0U;
    return (uint32_t)(((uint64_t)turns * 1000000U + length_mm / 2U) / length_mm);
}
