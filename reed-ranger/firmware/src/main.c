#include "pico/stdlib.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#define SENSOR 2
#define RUN_BUTTON 14
#define CLEAR_BUTTON 15
#define DEBOUNCE_US 30000

int main(void) {
    stdio_init_all();
    gpio_init(SENSOR); gpio_set_dir(SENSOR, GPIO_IN); gpio_pull_up(SENSOR);
    gpio_init(RUN_BUTTON); gpio_set_dir(RUN_BUTTON, GPIO_IN); gpio_pull_up(RUN_BUTTON);
    gpio_init(CLEAR_BUTTON); gpio_set_dir(CLEAR_BUTTON, GPIO_IN); gpio_pull_up(CLEAR_BUTTON);
    bool running = false, last_run = true, last_clear = true, was_closed = false;
    absolute_time_t last_edge = get_absolute_time(), last_close = get_absolute_time();
    uint32_t count = 0; int64_t open_start = 0;
    while (true) {
        bool r = gpio_get(RUN_BUTTON), c = gpio_get(CLEAR_BUTTON);
        if (last_run && !r) running = !running;
        if (last_clear && !c) { count = 0; last_close = get_absolute_time(); }
        last_run = r; last_clear = c;
        bool closed = !gpio_get(SENSOR);
        int64_t now = to_us_since_boot(get_absolute_time());
        if (!closed) open_start = now;
        if (running && closed && !was_closed && absolute_time_diff_us(last_edge, get_absolute_time()) >= DEBOUNCE_US) {
            last_edge = get_absolute_time();
            if (count != UINT32_MAX) ++count;
            int64_t closed_ms = open_start > 0 ? (now - open_start) / 1000 : 0;
            int64_t interval_ms = absolute_time_diff_us(last_close, get_absolute_time()) / 1000;
            if (count == 1) printf("{\"event\":\"closure\",\"count\":%" PRIu32 ",\"closed_ms\":%" PRId64 ",\"interval_ms\":null}\n", count, closed_ms);
            else printf("{\"event\":\"closure\",\"count\":%" PRIu32 ",\"closed_ms\":%" PRId64 ",\"interval_ms\":%" PRId64 "}\n", count, closed_ms, interval_ms);
            last_close = get_absolute_time();
        }
        was_closed = closed;
        sleep_ms(1);
    }
}
