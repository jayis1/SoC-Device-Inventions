#include <stdio.h>
#include "pico/stdlib.h"
#include "rain_core.h"

static rain_debounce_t debounce;
static volatile uint32_t edge_ms;
static volatile bool pending;
static void reed_irq(uint gpio, uint32_t events) {
    (void)gpio; (void)events;
    edge_ms = to_ms_since_boot(get_absolute_time());
    pending = true;
}
int main(void) {
    stdio_init_all();
    gpio_init(2); gpio_set_dir(2, GPIO_IN); gpio_pull_up(2);
    gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_FALL, true, &reed_irq);
    puts("rain-gauge-keeper,format=TIP,<uptime_ms>");
    while (true) {
        if (pending) {
            uint32_t t = edge_ms; pending = false;
            if (rain_accept_tip(&debounce, t)) printf("TIP,%lu\n", (unsigned long)t);
        }
        tight_loop_contents();
    }
}
