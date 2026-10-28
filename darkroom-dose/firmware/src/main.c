#ifndef PICO_DEFAULT_LED_PIN
#define PICO_DEFAULT_LED_PIN 25
#endif
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <stdio.h>

#define PHOTO_ADC 0u
#define OUTPUT_PIN 8u
#define START_PIN 6u
#define LIMIT_MS 120000u

int main(void) {
    stdio_init_all();
    adc_init();
    adc_gpio_init(26);
    gpio_init(OUTPUT_PIN); gpio_set_dir(OUTPUT_PIN, GPIO_OUT); gpio_put(OUTPUT_PIN, 0);
    gpio_init(START_PIN); gpio_set_dir(START_PIN, GPIO_IN); gpio_pull_up(START_PIN);
    printf("DARKROOM_DOSE,1\nready,raw_adc_counts\n");
    while (true) {
        if (!gpio_get(START_PIN)) {
            uint32_t start = to_ms_since_boot(get_absolute_time());
            uint64_t dose = 0;
            gpio_put(OUTPUT_PIN, 1);
            while (!gpio_get(START_PIN) && to_ms_since_boot(get_absolute_time()) - start < LIMIT_MS) {
                adc_select_input(PHOTO_ADC);
                dose += adc_read();
                sleep_ms(1);
            }
            gpio_put(OUTPUT_PIN, 0);
            printf("exposure_ms,%lu,dose_counts,%llu\n",
                   (unsigned long)(to_ms_since_boot(get_absolute_time()) - start),
                   (unsigned long long)dose);
            sleep_ms(300);
        }
        sleep_ms(10);
    }
}
