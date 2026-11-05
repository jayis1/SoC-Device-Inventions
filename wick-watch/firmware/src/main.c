#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"

#define START_PIN 14
#define LED_PIN 15
#define FIRST_SENSOR_PIN 2
#define SENSOR_COUNT 5
#define SAMPLE_MS 5
#define STABLE_SAMPLES 4
#define RUN_TIMEOUT_MS 120000u

static bool stable_low(uint pin) {
    unsigned low = 0;
    for (unsigned i = 0; i < STABLE_SAMPLES; ++i) {
        if (!gpio_get(pin)) ++low;
        sleep_ms(SAMPLE_MS);
    }
    return low == STABLE_SAMPLES;
}

int main(void) {
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(START_PIN);
    gpio_set_dir(START_PIN, GPIO_IN);
    gpio_pull_up(START_PIN);
    for (uint i = 0; i < SENSOR_COUNT; ++i) {
        gpio_init(FIRST_SENSOR_PIN + i);
        gpio_set_dir(FIRST_SENSOR_PIN + i, GPIO_IN);
        gpio_pull_up(FIRST_SENSOR_PIN + i);
    }
    sleep_ms(1500);
    printf("elapsed_ms,sensor,event\n");
    while (true) {
        gpio_put(LED_PIN, 0);
        while (gpio_get(START_PIN)) tight_loop_contents();
        sleep_ms(30);
        if (gpio_get(START_PIN)) continue;
        while (!gpio_get(START_PIN)) tight_loop_contents();
        const uint32_t start = to_ms_since_boot(get_absolute_time());
        bool seen[SENSOR_COUNT] = {false};
        unsigned remaining = SENSOR_COUNT;
        printf("0,0,start\n");
        gpio_put(LED_PIN, 1);
        while (remaining && (uint32_t)(to_ms_since_boot(get_absolute_time()) - start) < RUN_TIMEOUT_MS) {
            for (unsigned i = 0; i < SENSOR_COUNT; ++i) {
                const uint pin = FIRST_SENSOR_PIN + i;
                if (!seen[i] && !gpio_get(pin) && stable_low(pin)) {
                    const uint32_t elapsed = to_ms_since_boot(get_absolute_time()) - start;
                    printf("%lu,%u,arrival\n", (unsigned long)elapsed, i + 1);
                    seen[i] = true;
                    --remaining;
                }
            }
            tight_loop_contents();
        }
        if (remaining) printf("%lu,0,timeout\n", (unsigned long)RUN_TIMEOUT_MS);
        gpio_put(LED_PIN, 0);
        printf("%lu,0,end\n", (unsigned long)(to_ms_since_boot(get_absolute_time()) - start));
        sleep_ms(100);
    }
}
