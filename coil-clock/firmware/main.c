#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "detector.h"
#define PIEZO_PIN 15
int main(void) {
    stdio_init_all();
    adc_init(); adc_gpio_init(26); adc_select_input(0);
    gpio_init(PIEZO_PIN); gpio_set_dir(PIEZO_PIN, GPIO_OUT); gpio_put(PIEZO_PIN, 0);
    detector_t detector = {0};
    uint32_t count = 0, last_report = 0;
    while (true) {
        uint16_t sample = adc_read();
        uint32_t now = time_us_32();
        if (detector_update(&detector, sample, now)) {
            count++;
            gpio_put(PIEZO_PIN, 1); sleep_ms(8); gpio_put(PIEZO_PIN, 0);
            printf("{\"event\":\"turn\",\"count\":%lu,\"adc\":%u}\n", (unsigned long)count, sample);
        }
        if (to_ms_since_boot(get_absolute_time()) - last_report > 5000) {
            last_report = to_ms_since_boot(get_absolute_time());
            printf("{\"event\":\"status\",\"count\":%lu,\"adc\":%u}\n", (unsigned long)count, sample);
        }
        sleep_us(1000);
    }
}
