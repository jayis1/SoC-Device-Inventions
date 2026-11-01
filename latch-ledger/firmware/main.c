#include <stdio.h>
#include "pico/stdlib.h"

#define HALL_PIN 2
#define LED_PIN 25
#define DEBOUNCE_MS 30u

int main(void) {
    stdio_init_all();
    gpio_init(HALL_PIN);
    gpio_set_dir(HALL_PIN, GPIO_IN);
    gpio_pull_up(HALL_PIN); /* external 10k pull-up is specified too */
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    bool open = gpio_get(HALL_PIN) == 0;
    uint32_t last_change = to_ms_since_boot(get_absolute_time());
    uint32_t opened_at = last_change;
    sleep_ms(1500);
    printf("{\"event\":\"startup\",\"version\":1}\n");

    while (true) {
        uint32_t now = to_ms_since_boot(get_absolute_time());
        bool sensed_open = gpio_get(HALL_PIN) == 0;
        if (sensed_open != open && (uint32_t)(now - last_change) >= DEBOUNCE_MS) {
            open = sensed_open;
            last_change = now;
            gpio_put(LED_PIN, open);
            if (open) {
                opened_at = now;
                printf("{\"event\":\"open\",\"ms\":%lu}\n", (unsigned long)now);
            } else {
                printf("{\"event\":\"close\",\"ms\":%lu,\"dwell_ms\":%lu}\n",
                       (unsigned long)now, (unsigned long)(now - opened_at));
            }
            fflush(stdout);
        }
        sleep_ms(2);
    }
}
