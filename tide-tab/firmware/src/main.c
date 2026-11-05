#include "pico/stdlib.h"
#include <stdio.h>
#include "tide_tab.h"

static bool stable, initialized, candidate;
static uint32_t changed;
static void report(uint32_t ms, bool low, void *ctx) {
    (void)ctx;
    printf("{\"ms\":%lu,\"level_low\":%s}\n", (unsigned long)ms, low ? "true" : "false");
}
int main(void) {
    stdio_init_all();
    gpio_init(15); gpio_set_dir(15, GPIO_IN); gpio_pull_up(15);
    gpio_init(16); gpio_set_dir(16, GPIO_OUT); gpio_put(16, 0);
    while (true) {
        bool raw = !gpio_get(15);
        tide_tab_step(raw, to_ms_since_boot(get_absolute_time()), &stable,
                      &initialized, &candidate, &changed, report, NULL);
        gpio_put(16, stable);
        sleep_ms(10);
    }
}
