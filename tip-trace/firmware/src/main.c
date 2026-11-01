#include <stdint.h>
#include <stdbool.h>

/* Platform adapter supplies these functions in the final Pico SDK port. */
extern bool spi_read_max31855(uint32_t *word);
extern void emit_celsius(int32_t milli_celsius, bool fault);

/* Decode MAX31855 32-bit frame. Temperature fields are signed 14-bit, 0.25 C/LSB. */
bool tip_trace_sample(void) {
    uint32_t frame = 0;
    if (!spi_read_max31855(&frame)) return false;
    bool fault = (frame & 0x00010007u) != 0u;
    int32_t raw = (int32_t)((frame >> 18) & 0x3fffu);
    if ((raw & 0x2000) != 0) raw -= 0x4000;
    emit_celsius(raw * 250, fault);
    return true;
}
