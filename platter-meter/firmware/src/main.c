#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "platter_meter.h"
#include <stdio.h>

#define SENSOR_PIN 2
static volatile uint32_t last_us;
static volatile unsigned intervals[8];
static volatile unsigned count;
static volatile bool ready;
static void edge(uint gpio, uint32_t events) {
    (void)gpio; (void)events;
    uint32_t now = time_us_32(), prev = last_us;
    last_us = now;
    if (ready) return;
    if (!prev) return;
    uint32_t delta = now - prev;
    if (delta < 500000 || delta > 7000000) { count = 0; ready = false; return; }
    intervals[count++] = delta / 1000;
    if (count == 8) { ready = true; count = 0; }
}
int main(void) {
    stdio_init_all();
    gpio_init(SENSOR_PIN); gpio_set_dir(SENSOR_PIN, GPIO_IN); gpio_pull_up(SENSOR_PIN);
    gpio_set_irq_enabled_with_callback(SENSOR_PIN, GPIO_IRQ_EDGE_FALL, true, &edge);
    uint32_t last_report = 0;
    for (;;) {
        if (ready && (time_us_32() - last_report) > 200000) {
            unsigned local[8];
            uint32_t irq = save_and_disable_interrupts();
            for (unsigned i=0;i<8;i++) local[i] = intervals[i];
            ready = false;
            restore_interrupts(irq);
            result_t r;
            if (analyze_intervals(local, 8, &r)) {
                printf("{\"rpm\":%.3f,\"interval_cv_pct\":%.3f,\"samples\":%u}\n", r.rpm, r.interval_cv_pct, (unsigned)r.samples);
                last_report = time_us_32();
            }
        }
        tight_loop_contents();
    }
}
