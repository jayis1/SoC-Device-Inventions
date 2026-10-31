#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define ENC_A 10
#define ENC_B 11
#define ENC_SW 12
#define HX_DOUT 6
#define HX_SCK 7

static volatile int32_t ticks;
static void encoder_irq(uint gpio, uint32_t events) {
    (void)events;
    bool a = gpio_get(ENC_A), b = gpio_get(ENC_B);
    ticks += (gpio == ENC_A) ? (a == b ? 1 : -1) : (a != b ? 1 : -1);
}
static bool hx711_read(int32_t *out) {
    absolute_time_t until = make_timeout_time_ms(250);
    while (gpio_get(HX_DOUT)) { if (time_reached(until)) return false; tight_loop_contents(); }
    uint32_t v = 0;
    for (int i = 0; i < 24; ++i) {
        gpio_put(HX_SCK, 1); sleep_us(1);
        v = (v << 1) | gpio_get(HX_DOUT);
        gpio_put(HX_SCK, 0); sleep_us(1);
    }
    gpio_put(HX_SCK, 1); sleep_us(1); gpio_put(HX_SCK, 0); // gain 128, channel A
    *out = (v & 0x800000u) ? (int32_t)(v | 0xff000000u) : (int32_t)v;
    return true;
}
int main(void) {
    stdio_init_all();
    gpio_init(HX_DOUT); gpio_set_dir(HX_DOUT, GPIO_IN); gpio_pull_up(HX_DOUT);
    gpio_init(HX_SCK); gpio_set_dir(HX_SCK, GPIO_OUT); gpio_put(HX_SCK, 0);
    for (uint pin = ENC_A; pin <= ENC_SW; ++pin) { gpio_init(pin); gpio_set_dir(pin, GPIO_IN); gpio_pull_up(pin); }
    gpio_set_irq_enabled_with_callback(ENC_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_irq);
    gpio_set_irq_enabled(ENC_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    puts("ms,angle_deg,raw_adc,force_n");
    while (true) {
        int32_t raw;
        if (hx711_read(&raw)) printf("%llu,%.2f,%ld,\n", (unsigned long long)to_ms_since_boot(get_absolute_time()), (double)ticks * 0.25, (long)raw);
        else puts("# hx711_timeout");
        sleep_ms(50);
    }
}
