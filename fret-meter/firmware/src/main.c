#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "fret_meter.h"
#include <stdio.h>
int main(void) {
    stdio_init_all(); adc_init(); adc_gpio_init(26); adc_select_input(0);
    gpio_init(14); gpio_set_dir(14, GPIO_IN); gpio_pull_up(14);
    fm_state state; fm_init(&state, 0.0f, 10.0f);
    while (true) {
        if (!gpio_get(14)) {
            sleep_ms(20);
            if (!gpio_get(14)) {
                state.sum = 0; state.count = 0;
                for (unsigned i=0; i<16; ++i) { fm_push(&state, adc_read()); sleep_ms(2); }
                uint16_t raw; float mm;
                if (fm_reading(&state, &raw, &mm)) printf("{\"raw\":%u,\"relative_mm\":%.3f}\r\n", raw, mm);
                while (!gpio_get(14)) sleep_ms(5);
            }
        }
        sleep_ms(5);
    }
}
