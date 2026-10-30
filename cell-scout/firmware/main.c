# Copyright (c) jayis1. All rights reserved.
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define LOAD_PIN 15u
#define ADC_PIN 26u
#define ADC_INPUT 0u
#define DIVIDER_RATIO 2.0f
#define ADC_REF_V 3.3f
#define ADC_COUNTS 4095.0f
#define LOAD_OHMS 100.0f

static float sample_cell_voltage(void) {
    adc_select_input(ADC_INPUT);
    sleep_ms(5);
    const uint16_t raw = adc_read();
    return ((float)raw * ADC_REF_V / ADC_COUNTS) * DIVIDER_RATIO;
}

int main(void) {
    stdio_init_all();
    gpio_init(LOAD_PIN);
    gpio_put(LOAD_PIN, 0);
    gpio_set_dir(LOAD_PIN, GPIO_OUT);
    adc_init();
    adc_gpio_init(ADC_PIN);
    while (true) {
        int ch;
        printf("Cell Scout ready; send t to test\n");
        ch = getchar_timeout_us(1000000);
        if (ch != 't' && ch != 'T') continue;
        const float open_v = sample_cell_voltage();
        gpio_put(LOAD_PIN, 1);
        sleep_ms(100);
        const float loaded_v = sample_cell_voltage();
        gpio_put(LOAD_PIN, 0);
        sleep_ms(100);
        const float recovery_v = sample_cell_voltage();
        const float estimated_ma = loaded_v / LOAD_OHMS * 1000.0f;
        const float resistance = estimated_ma > 0.1f ? (open_v - loaded_v) / (estimated_ma / 1000.0f) : 0.0f;
        printf("open_V,loaded_V,recovery_V,estimated_mA,approx_mOhm\n%.3f,%.3f,%.3f,%.1f,%.1f\n",
               open_v, loaded_v, recovery_v, estimated_ma, resistance * 1000.0f);
    }
}
