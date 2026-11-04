#include <stdio.h>
#include "core.h"
void csv_emit(FILE *out, uint32_t ms, int32_t force, uint16_t angle) {
    (void)fprintf(out, "%lu,%ld,%u\n", (unsigned long)ms, (long)force, (unsigned)angle);
}
