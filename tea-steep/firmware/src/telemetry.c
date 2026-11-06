#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* Platform-independent telemetry formatter; hardware adapters live in main.c. */
const char *tea_steep_csv_header(void) {
    return "ms,temp_c,r,g,b,clear,status\r\n";
}

int tea_steep_format_row(char *out, size_t capacity, uint32_t ms,
                         int temp_milli_c, int temp_ok,
                         uint16_t r, uint16_t g, uint16_t b, uint16_t clear,
                         int color_ok) {
    if (!out || capacity == 0) return -1;
    if (!temp_ok || !color_ok) {
        return snprintf(out, capacity, "%lu,,%u,%u,%u,%u,ERR\r\n",
                        (unsigned long)ms, r, g, b, clear);
    }
    return snprintf(out, capacity, "%lu,%d.%03d,%u,%u,%u,%u,OK\r\n",
                    (unsigned long)ms, temp_milli_c / 1000,
                    (temp_milli_c < 0 ? -temp_milli_c : temp_milli_c) % 1000,
                    r, g, b, clear);
}
