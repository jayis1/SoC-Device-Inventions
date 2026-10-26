#include <stdbool.h>
#include <stdint.h>
#include "pico/stdlib.h"

#define SENSOR_PIN 14
#define SWITCH_PIN 15
#define SHUTTER_PIN 16
#ifndef FILM_GATE_ENABLE_SHUTTER
#define FILM_GATE_ENABLE_SHUTTER 0
#endif

static bool stable(bool expected, uint32_t ms) {
    absolute_time_t until = make_timeout_time_ms(ms);
    while (!time_reached(until)) {
        if (gpio_get(SENSOR_PIN) != expected) return false;
        sleep_ms(1);
    }
    return true;
}

int main(void) {
    stdio_init_all();
    gpio_init(SENSOR_PIN); gpio_set_dir(SENSOR_PIN, GPIO_IN); gpio_pull_up(SENSOR_PIN);
    gpio_init(SWITCH_PIN); gpio_set_dir(SWITCH_PIN, GPIO_IN); gpio_pull_up(SWITCH_PIN);
    gpio_init(SHUTTER_PIN); gpio_set_dir(SHUTTER_PIN, GPIO_OUT); gpio_put(SHUTTER_PIN, 0);
    bool last_sensor = gpio_get(SENSOR_PIN);
    while (true) {
        bool sensor = gpio_get(SENSOR_PIN);
        if (sensor != last_sensor && stable(sensor, 30)) {
            last_sensor = sensor;
            printf("%s\n", sensor ? "ADVANCE" : "READY");
        }
        if (!gpio_get(SWITCH_PIN) && stable(false, 30)) {
            if (last_sensor) {
                printf("FAULT,film-not-at-gap\n");
            } else {
                printf("BUSY\n");
#if FILM_GATE_ENABLE_SHUTTER
                gpio_put(SHUTTER_PIN, 1); sleep_ms(150); gpio_put(SHUTTER_PIN, 0);
#endif
                printf("SHOT\n"); sleep_ms(500);
            }
            while (!gpio_get(SWITCH_PIN)) sleep_ms(5);
        }
        sleep_ms(2);
    }
}
