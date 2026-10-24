#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "matrix.h"
#include <stdio.h>

static const uint drive_sel[3] = {2, 3, 4};
static const uint sense_sel[3] = {6, 7, 8};

static void select_mux(const uint pins[3], unsigned channel) {
    for (unsigned bit = 0; bit < 3; ++bit) gpio_put(pins[bit], (channel >> bit) & 1u);
}

int main(void) {
    stdio_init_all();
    for (unsigned i = 0; i < 3; ++i) {
        gpio_init(drive_sel[i]); gpio_set_dir(drive_sel[i], GPIO_OUT); gpio_put(drive_sel[i], 0);
        gpio_init(sense_sel[i]); gpio_set_dir(sense_sel[i], GPIO_OUT); gpio_put(sense_sel[i], 0);
    }
    gpio_init(5); gpio_set_dir(5, GPIO_OUT); gpio_put(5, 1); /* disabled */
    adc_init(); adc_gpio_init(26); adc_select_input(0);
    while (true) {
        uint8_t samples[8][8] = {{0}};
        for (unsigned d = 0; d < 8; ++d) {
            select_mux(drive_sel, d);
            gpio_put(5, 0);
            sleep_us(100);
            for (unsigned s = 0; s < 8; ++s) {
                select_mux(sense_sel, s); sleep_us(50);
                samples[d][s] = adc_read() > 1800;
            }
            gpio_put(5, 1);
        }
        int8_t result[8][8]; matrix_classify(samples, result);
        printf("{\"contacts\":8,\"matrix\":[");
        for (unsigned d = 0; d < 8; ++d) {
            printf("%s[", d ? "," : "");
            for (unsigned s = 0; s < 8; ++s) printf("%s%d", s ? "," : "", result[d][s]);
            printf("]");
        }
        printf("]}\n"); sleep_ms(500);
    }
}