#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define ADC_CS 17
#define ADC_MISO 16
#define ADC_SCK 18
#define ADC_MOSI 19

static unsigned short read_adc(unsigned char channel) {
    unsigned char tx[3] = { (unsigned char)(0x01), (unsigned char)(0x80 | (channel << 4)), 0 };
    unsigned char rx[3] = {0};
    gpio_put(ADC_CS, 0);
    spi_write_read_blocking(spi0, tx, rx, 3);
    gpio_put(ADC_CS, 1);
    return (unsigned short)(((rx[1] & 0x03) << 8) | rx[2]);
}

int main(void) {
    stdio_init_all();
    spi_init(spi0, 1000000);
    gpio_set_function(ADC_MISO, GPIO_FUNC_SPI);
    gpio_set_function(ADC_SCK, GPIO_FUNC_SPI);
    gpio_set_function(ADC_MOSI, GPIO_FUNC_SPI);
    gpio_init(ADC_CS);
    gpio_set_dir(ADC_CS, GPIO_OUT);
    gpio_put(ADC_CS, 1);
    sleep_ms(1500);
    printf("sample,adc_raw\n");
    unsigned long sample = 0;
    while (true) {
        printf("%lu,%u\n", sample++, read_adc(0));
        sleep_ms(250);
    }
}
