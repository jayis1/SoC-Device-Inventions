#include <stdio.h>
#include "pico/stdlib.h"

#define PIN_SENSOR 15
#define PIN_IR_LED 12
#define PIN_STATUS 17
#define DEBOUNCE_MS 80u

static uint32_t last_edge_ms;
static uint32_t revolutions;
static bool previous_high;

int main(void) {
    stdio_init_all();
    gpio_init(PIN_SENSOR);
    gpio_set_dir(PIN_SENSOR, GPIO_IN);
    gpio_pull_up(PIN_SENSOR);
    gpio_init(PIN_IR_LED);
    gpio_set_dir(PIN_IR_LED, GPIO_OUT);
    gpio_put(PIN_IR_LED, 1);
    gpio_init(PIN_STATUS);
    gpio_set_dir(PIN_STATUS, GPIO_OUT);
    previous_high = gpio_get(PIN_SENSOR);
    sleep_ms(1200);
    while (true) {
        const bool high = gpio_get(PIN_SENSOR);
        const uint32_t now = to_ms_since_boot(get_absolute_time());
        if (previous_high && !high && (uint32_t)(now - last_edge_ms) >= DEBOUNCE_MS) {
            last_edge_ms = now;
            ++revolutions;
            gpio_put(PIN_STATUS, 1);
            printf("{\"t_ms\":%lu,\"revolutions\":%lu}\n", (unsigned long)now, (unsigned long)revolutions);
            fflush(stdout);
            gpio_put(PIN_STATUS, 0);
        }
        previous_high = high;
        sleep_ms(1);
    }
}
