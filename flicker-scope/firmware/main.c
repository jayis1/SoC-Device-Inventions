#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define ADC_GPIO 26
#define RATE_HZ 500u
#define BLOCK 256u

int main(void) {
    stdio_init_all();
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    adc_init();
    adc_gpio_init(ADC_GPIO);
    adc_select_input(0);
    uint32_t blocks = 0;
    absolute_time_t next = get_absolute_time();
    while (true) {
        for (unsigned i = 0; i < BLOCK; ++i) {
            const uint16_t value = adc_read();
            printf("sample,%llu,%u\n", (unsigned long long)to_us_since_boot(get_absolute_time()), value);
            next = delayed_by_us(next, 1000000u / RATE_HZ);
            sleep_until(next);
        }
        if ((++blocks & 3u) == 0u)
            gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));
    }
}
