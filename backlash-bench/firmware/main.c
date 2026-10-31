#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_SDA 4
#define I2C_SCL 5
#define DIR_PIN 6
#define LED_PIN 25
#define MUX_ADDR 0x70
#define SENSOR_ADDR 0x36
#define REG_ANGLE 0x0E

static bool select_sensor(unsigned channel) {
    uint8_t mask = (uint8_t)(1u << channel);
    return i2c_write_blocking(i2c0, MUX_ADDR, &mask, 1, false) == 1;
}
static bool read_angle(unsigned channel, uint16_t *raw) {
    uint8_t reg = REG_ANGLE, data[2];
    if (!select_sensor(channel) || i2c_write_blocking(i2c0, SENSOR_ADDR, &reg, 1, true) != 1) return false;
    if (i2c_read_blocking(i2c0, SENSOR_ADDR, data, 2, false) != 2) return false;
    *raw = (uint16_t)(((data[0] & 0x0f) << 8) | data[1]);
    return true;
}
static float degrees(uint16_t raw) { return 360.0f * (float)raw / 4096.0f; }

int main(void) {
    stdio_init_all();
    i2c_init(i2c0, 100000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA); gpio_pull_up(I2C_SCL);
    gpio_init(DIR_PIN); gpio_set_dir(DIR_PIN, GPIO_IN); gpio_pull_up(DIR_PIN);
    gpio_init(LED_PIN); gpio_set_dir(LED_PIN, GPIO_OUT);
    sleep_ms(1500);
    puts("elapsed_us,input_deg,output_deg,direction");
    absolute_time_t start = get_absolute_time();
    while (true) {
        uint16_t in, out;
        bool ok = read_angle(0, &in) && read_angle(1, &out);
        gpio_put(LED_PIN, ok);
        if (ok) printf("%lld,%.3f,%.3f,%u\n", (long long)absolute_time_diff_us(start, get_absolute_time()), degrees(in), degrees(out), gpio_get(DIR_PIN) ? 0u : 1u);
        else puts("ERROR,0,0,0");
        sleep_ms(50);
    }
}
