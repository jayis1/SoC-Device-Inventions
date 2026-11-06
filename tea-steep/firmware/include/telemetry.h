#pragma once
#include <stddef.h>
#include <stdint.h>
const char *tea_steep_csv_header(void);
int tea_steep_format_row(char *out, size_t capacity, uint32_t ms,
                         int temp_milli_c, int temp_ok,
                         uint16_t r, uint16_t g, uint16_t b, uint16_t clear,
                         int color_ok);
