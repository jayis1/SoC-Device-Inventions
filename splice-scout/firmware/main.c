#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define MARK_PIN 14u
#define INDEX_PIN 15u
#define DEBOUNCE_MS 10u

static void emit_event(const char *event, uint16_t raw) {
    printf("%llu,%s,%u\n", (unsigned long long)to_ms_since_boot(get_absolute_time()), event, raw);
    fflush(stdout);
}

int main(void) {
    stdio_init_all();
    adc_init();
    adc_gpio_init(26u);
    gpio_init(MARK_PIN);
    gpio_set_dir(MARK_PIN, GPIO_IN);
    gpio_pull_up(MARK_PIN);
    gpio_init(INDEX_PIN);
    gpio_set_dir(INDEX_PIN, GPIO_IN);
    gpio_pull_up(INDEX_PIN);

    bool mark_ready = true, index_ready = true;
    absolute_time_t mark_last = get_absolute_time();
    absolute_time_t index_last = mark_last;
    while (true) {
        const absolute_time_t now = get_absolute_time();
        if (gpio_get(MARK_PIN)) mark_ready = true;
        else if (mark_ready && absolute_time_diff_us(mark_last, now) >= (int64_t)DEBOUNCE_MS * 1000) {
            adc_select_input(0u);
            emit_event("mark", adc_read());
            mark_ready = false;
            mark_last = now;
        }
        if (gpio_get(INDEX_PIN)) index_ready = true;
        else if (index_ready && absolute_time_diff_us(index_last, now) >= (int64_t)DEBOUNCE_MS * 1000) {
            adc_select_input(0u);
            emit_event("index", adc_read());
            index_ready = false;
            index_last = now;
        }
        sleep_ms(1u);
    }
}
