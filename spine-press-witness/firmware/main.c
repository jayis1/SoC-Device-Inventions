/* Raspberry Pi Pico SDK 2.x; RP2040. Design reference only, not target-built here. */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define HX_DOUT 14
#define HX_SCK 15
#define HX_TIMEOUT_US 100000u
#define SAMPLE_PERIOD_MS 100u

static bool hx_read(int32_t *out) {
    absolute_time_t deadline = make_timeout_time_us(HX_TIMEOUT_US);
    while (gpio_get(HX_DOUT)) {
        if (time_reached(deadline)) return false;
        tight_loop_contents();
    }
    uint32_t value = 0;
    for (unsigned i = 0; i < 24; ++i) {
        gpio_put(HX_SCK, 1);
        busy_wait_us_32(1);
        value = (value << 1) | (uint32_t)gpio_get(HX_DOUT);
        gpio_put(HX_SCK, 0);
        busy_wait_us_32(1);
    }
    /* 25th pulse selects channel A, gain 128 for next conversion. */
    gpio_put(HX_SCK, 1);
    busy_wait_us_32(1);
    gpio_put(HX_SCK, 0);
    busy_wait_us_32(1);
    if (value & 0x800000u) value |= 0xff000000u;
    *out = (int32_t)value;
    return true;
}

int main(void) {
    stdio_init_all();
    gpio_init(HX_DOUT);
    gpio_set_dir(HX_DOUT, GPIO_IN);
    gpio_disable_pulls(HX_DOUT);
    gpio_init(HX_SCK);
    gpio_set_dir(HX_SCK, GPIO_OUT);
    gpio_put(HX_SCK, 0);
    int32_t offset = 0;
    bool have_offset = false;
    uint32_t sequence = 0;
    while (true) {
        int32_t raw;
        if (!hx_read(&raw)) {
            puts("sensor_error");
        } else {
            if (!have_offset) { offset = raw; have_offset = true; }
            int c = getchar_timeout_us(0);
            if (c == 't' && have_offset) offset = raw;
            printf("%u,%ld,%ld\n", to_ms_since_boot(get_absolute_time()),
                   (long)raw, (long)(raw - offset));
            ++sequence;
            (void)sequence;
        }
        sleep_ms(SAMPLE_PERIOD_MS);
    }
}
