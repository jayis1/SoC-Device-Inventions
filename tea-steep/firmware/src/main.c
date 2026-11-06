#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "telemetry.h"
#include <stdio.h>

#define SDA_PIN 4
#define SCL_PIN 5
#define LED_PIN 15
#define TCS_ADDR 0x29
static bool tcs_read(uint8_t reg, uint8_t *dst, size_t n) {
    uint8_t command = (uint8_t)(0x80u | reg);
    if (i2c_write_blocking(i2c0, TCS_ADDR, &command, 1, true) != 1) return false;
    return i2c_read_blocking(i2c0, TCS_ADDR, dst, n, false) == (int)n;
}
int main(void) {
    stdio_init_all();
    i2c_init(i2c0, 100000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C); gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN); gpio_pull_up(SCL_PIN);
    gpio_init(LED_PIN); gpio_set_dir(LED_PIN, GPIO_OUT); gpio_put(LED_PIN, 0);
    /* TCS34725 command register: power on, then RGBC ADC enable. */
    uint8_t init1[] = {0x80, 0x01}, init2[] = {0x80, 0x03}, integration[] = {0x81, 0xD5};
    sleep_ms(500);
    bool initialized = i2c_write_blocking(i2c0, TCS_ADDR, init1, 2, false) == 2;
    sleep_ms(3);
    initialized &= i2c_write_blocking(i2c0, TCS_ADDR, init2, 2, false) == 2;
    initialized &= i2c_write_blocking(i2c0, TCS_ADDR, integration, 2, false) == 2;
    puts(tea_steep_csv_header());
    while (true) {
        gpio_put(LED_PIN, 1); sleep_ms(20);
        uint8_t raw[8] = {0};
        bool ok = initialized && tcs_read(0x14, raw, sizeof raw);
        gpio_put(LED_PIN, 0);
        uint16_t c=(uint16_t)(raw[1]<<8|raw[0]), r=(uint16_t)(raw[3]<<8|raw[2]);
        uint16_t g=(uint16_t)(raw[5]<<8|raw[4]), b=(uint16_t)(raw[7]<<8|raw[6]);
        char line[96];
        int len=tea_steep_format_row(line,sizeof line,to_ms_since_boot(get_absolute_time()),0,0,r,g,b,c,ok);
        if (len>0 && (size_t)len<sizeof line) printf("%s",line);
        sleep_ms(980);
    }
}
