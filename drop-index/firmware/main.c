#include <stdio.h>
#include "pico/stdlib.h"

#define HX_DOUT 2
#define HX_SCK 3
#define START_PIN 15
#define SAMPLE_COUNT 5
#define COUNTS_PER_GRAM 1000.0f /* Replace after calibration; illustrative only. */

static bool hx_read(int32_t *out) {
    absolute_time_t deadline = make_timeout_time_ms(250);
    while (gpio_get(HX_DOUT)) {
        if (time_reached(deadline)) return false;
        tight_loop_contents();
    }
    int32_t value = 0;
    for (int i = 0; i < 24; ++i) {
        gpio_put(HX_SCK, 1); busy_wait_us_32(1);
        value = (value << 1) | gpio_get(HX_DOUT);
        gpio_put(HX_SCK, 0); busy_wait_us_32(1);
    }
    gpio_put(HX_SCK, 1); busy_wait_us_32(1); gpio_put(HX_SCK, 0); /* gain 128 */
    if (value & 0x800000) value |= (int32_t)0xff000000;
    *out = value;
    return true;
}

static bool average_counts(int32_t *out) {
    int64_t sum = 0;
    for (int i = 0; i < SAMPLE_COUNT; ++i) {
        int32_t v;
        if (!hx_read(&v)) return false;
        sum += v;
    }
    *out = (int32_t)(sum / SAMPLE_COUNT);
    return true;
}

int main(void) {
    stdio_init_all();
    gpio_init(HX_DOUT); gpio_set_dir(HX_DOUT, GPIO_IN); gpio_pull_up(HX_DOUT);
    gpio_init(HX_SCK); gpio_set_dir(HX_SCK, GPIO_OUT); gpio_put(HX_SCK, 0);
    gpio_init(START_PIN); gpio_set_dir(START_PIN, GPIO_IN); gpio_pull_up(START_PIN);
    sleep_ms(1500);
    printf("Drop Index;serial CSV;reference firmware\npress button to record 60 seconds\n");
    int32_t tare;
    if (!average_counts(&tare)) { printf("ERROR,HX711 timeout\n"); while (true) sleep_ms(1000); }
    while (true) {
        if (!gpio_get(START_PIN)) {
            sleep_ms(30);
            if (!gpio_get(START_PIN)) {
                printf("elapsed_s,mass_g,rate_g_per_min\n");
                int32_t previous = tare;
                absolute_time_t start = get_absolute_time();
                for (int s = 1; s <= 60; ++s) {
                    sleep_until(delayed_by_ms(start, s * 1000));
                    int32_t now;
                    if (!average_counts(&now)) { printf("ERROR,HX711 timeout\n"); break; }
                    float mass = (now - tare) / COUNTS_PER_GRAM;
                    float delta = (now - previous) / COUNTS_PER_GRAM;
                    printf("%d,%.3f,%.3f\n", s, mass, delta * 60.0f);
                    previous = now;
                }
                while (!gpio_get(START_PIN)) sleep_ms(10);
            }
        }
        sleep_ms(10);
    }
}
