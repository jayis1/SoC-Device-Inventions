#include <stdio.h>
#include "pico/stdlib.h"

#define CYCLE_PIN 2
#define FEED_PIN 3
#define BUZZER_PIN 14
#define LED_PIN 15

static volatile uint32_t cycles, feeds;
static volatile uint64_t last_cycle_us, last_feed_us;

static void edge_irq(uint gpio, uint32_t events) {
    if ((events & GPIO_IRQ_EDGE_FALL) == 0) return;
    uint64_t now = time_us_64();
    if (gpio == CYCLE_PIN && now - last_cycle_us >= 20000) {
        last_cycle_us = now;
        cycles++;
    } else if (gpio == FEED_PIN && now - last_feed_us >= 20000) {
        last_feed_us = now;
        feeds++;
    }
}

int main(void) {
    stdio_init_all();
    gpio_init(CYCLE_PIN); gpio_set_dir(CYCLE_PIN, GPIO_IN); gpio_pull_up(CYCLE_PIN);
    gpio_init(FEED_PIN); gpio_set_dir(FEED_PIN, GPIO_IN); gpio_pull_up(FEED_PIN);
    gpio_init(LED_PIN); gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BUZZER_PIN); gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_set_irq_enabled_with_callback(CYCLE_PIN, GPIO_IRQ_EDGE_FALL, true, &edge_irq);
    gpio_set_irq_enabled(FEED_PIN, GPIO_IRQ_EDGE_FALL, true);
    uint32_t previous_cycles = 0, previous_feeds = 0;
    unsigned stale_windows = 0;
    absolute_time_t next = make_timeout_time_ms(250);
    puts("ms,event,cycles,feeds,alarm");
    while (true) {
        sleep_until(next); next = delayed_by_ms(next, 250);
        uint32_t c, f;
        uint32_t irq = save_and_disable_interrupts(); c = cycles; f = feeds; restore_interrupts(irq);
        bool mismatch = c > previous_cycles && f == previous_feeds;
        stale_windows = mismatch ? stale_windows + 1 : 0;
        bool alarm = stale_windows >= 3;
        gpio_put(LED_PIN, alarm);
        gpio_put(BUZZER_PIN, alarm);
        printf("%llu,%s,%u,%u,%u\n", (unsigned long long)(time_us_64()/1000),
               mismatch ? "MISMATCH" : (c != previous_cycles ? "CYCLE" : "IDLE"), c, f, alarm ? 1u : 0u);
        previous_cycles = c; previous_feeds = f;
    }
}
