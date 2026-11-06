#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define HX_DOUT 2
#define HX_SCK 3
#define FOOT 14
#define SAMPLE_US 20000

static bool hx_read(int32_t *out) {
    absolute_time_t deadline = make_timeout_time_ms(100);
    while (gpio_get(HX_DOUT)) {
        if (time_reached(deadline)) return false;
        tight_loop_contents();
    }
    uint32_t value = 0;
    for (int i = 0; i < 24; ++i) {
        gpio_put(HX_SCK, 1); sleep_us(1);
        value = (value << 1) | (gpio_get(HX_DOUT) ? 1u : 0u);
        gpio_put(HX_SCK, 0); sleep_us(1);
    }
    // One extra clock selects HX711 channel A, gain 128 for the next conversion.
    gpio_put(HX_SCK, 1); sleep_us(1); gpio_put(HX_SCK, 0);
    if (value & 0x800000u) value |= 0xff000000u;
    *out = (int32_t)value;
    return true;
}

int main(void) {
    stdio_init_all();
    gpio_init(HX_DOUT); gpio_set_dir(HX_DOUT, GPIO_IN); gpio_pull_up(HX_DOUT);
    gpio_init(HX_SCK); gpio_set_dir(HX_SCK, GPIO_OUT); gpio_put(HX_SCK, 0);
    gpio_init(FOOT); gpio_set_dir(FOOT, GPIO_IN); gpio_pull_up(FOOT);
    sleep_ms(1500);
    puts("ms,raw,peak_raw,event");
    int32_t peak = 0;
    bool previous_pressed = false;
    absolute_time_t next = get_absolute_time();
    while (true) {
        next = delayed_by_us(next, SAMPLE_US);
        sleep_until(next);
        bool pressed = !gpio_get(FOOT);
        if (pressed && !previous_pressed) { peak = 0; puts("0,0,0,reset"); }
        previous_pressed = pressed;
        int32_t raw;
        if (!hx_read(&raw)) { puts("0,0,0,hx_timeout"); continue; }
        if (raw > peak) peak = raw;
        printf("%lld,%ld,%ld,%s\n", (long long)to_ms_since_boot(get_absolute_time()),
               (long)raw, (long)peak, pressed ? "pressed" : "sample");
    }
}
