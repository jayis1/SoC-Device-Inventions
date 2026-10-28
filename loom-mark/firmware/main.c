#include <stdio.h>
#include "pico/stdlib.h"

#define GATE_A 2
#define GATE_B 3
#define DEBOUNCE_MS 35u

typedef enum { WAIT_A, WAIT_B } state_t;

int main(void) {
    stdio_init_all();
    gpio_init(GATE_A); gpio_set_dir(GATE_A, GPIO_IN); gpio_pull_up(GATE_A);
    gpio_init(GATE_B); gpio_set_dir(GATE_B, GPIO_IN); gpio_pull_up(GATE_B);
    gpio_init(PICO_DEFAULT_LED_PIN); gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    state_t state = WAIT_A;
    uint32_t last = 0, pick = 0;
    while (true) {
        uint32_t now = to_ms_since_boot(get_absolute_time());
        bool a = !gpio_get(GATE_A), b = !gpio_get(GATE_B);
        if (now - last >= DEBOUNCE_MS) {
            if (state == WAIT_A && a && !b) { state = WAIT_B; last = now; }
            else if (state == WAIT_B && b) {
                ++pick; printf("%lu,%lu\n", (unsigned long)pick, (unsigned long)now);
                state = WAIT_A; last = now;
                gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));
            } else if (state == WAIT_B && now - last > 1000u) state = WAIT_A;
        }
        tight_loop_contents();
    }
}
