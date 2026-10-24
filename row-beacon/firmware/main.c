/* Pico SDK adapter scaffold. Display and encoder integration are not implemented. */
#include "pico/stdlib.h"
#include "row_core.h"

#define HALL_GPIO 2

int main(void) {
    stdio_init_all();
    gpio_init(HALL_GPIO);
    gpio_set_dir(HALL_GPIO, GPIO_IN);
    gpio_pull_up(HALL_GPIO);
    row_core_t counter;
    row_core_init(&counter, 35u);
    bool previous = gpio_get(HALL_GPIO) != 0;
    while (true) {
        bool current = gpio_get(HALL_GPIO) != 0;
        if (previous && !current) {
            row_core_sample(&counter, false, to_ms_since_boot(get_absolute_time()));
        } else {
            row_core_sample(&counter, current, to_ms_since_boot(get_absolute_time()));
        }
        previous = current;
        tight_loop_contents();
    }
}
