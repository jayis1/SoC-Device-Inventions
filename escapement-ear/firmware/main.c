#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "escapement.h"
#include "config.h"
#include <stdio.h>
#define TICK_GPIO 14
#define TARGET_US EE_TARGET_PERIOD_US
static ee_stats_t stats;
static void tick_irq(uint gpio, uint32_t events) { (void)gpio; (void)events; ee_edge(&stats, time_us_32()); }
int main(void) {
    stdio_init_all(); ee_init(&stats);
    gpio_init(TICK_GPIO); gpio_set_dir(TICK_GPIO, GPIO_IN); gpio_pull_down(TICK_GPIO);
    gpio_set_irq_enabled_with_callback(TICK_GPIO, GPIO_IRQ_EDGE_RISE, true, &tick_irq);
    while (true) {
        sleep_ms(1000);
        uint32_t period=ee_median_interval(&stats);
        int32_t rate=ee_rate_seconds_per_day(&stats,TARGET_US);
        printf("interval_us=%lu rate_s_day=%ld samples=%u\n", (unsigned long)period,(long)rate,stats.count);
    }
}
