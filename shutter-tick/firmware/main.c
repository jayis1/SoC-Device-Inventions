#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "timing.h"
#define SENSOR_PIN 15
#define LED_PIN 25
int main(void) {
    stdio_init_all();
    gpio_init(SENSOR_PIN); gpio_set_dir(SENSOR_PIN, GPIO_IN); gpio_pull_up(SENSOR_PIN);
    gpio_init(LED_PIN); gpio_set_dir(LED_PIN, GPIO_OUT);
    shutter_capture_t state = {.waiting_for_rise = true, .rise_us = 0};
    uint64_t deadline = time_us_64() + 1000000u;
    while (true) {
        /* With collector pull-up, illumination pulls the node low. */
        bool light_present = !gpio_get(SENSOR_PIN);
        if (state.waiting_for_rise && light_present)
            deadline = time_us_64() + 1000000u;
        uint32_t duration;
        if (shutter_capture_edge(&state, light_present, time_us_64(), &duration)) {
            printf("{\"duration_us\":%lu,\"valid\":true}\n", (unsigned long)duration);
            gpio_put(LED_PIN, 1);
            gpio_put(LED_PIN, 0);
        }
        if (!state.waiting_for_rise && time_us_64() > deadline) {
            puts("{\"valid\":false,\"error\":\"pulse_timeout\"}");
            state.waiting_for_rise = true;
            deadline = time_us_64() + 1000000u;
        }
        tight_loop_contents();
    }
}
