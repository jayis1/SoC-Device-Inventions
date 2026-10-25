#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define SENSOR_ADDR 0x29
#define REG_ENABLE 0x80
#define REG_ATIME 0x81
#define REG_CONTROL 0x8F
#define REG_ID 0x92
#define REG_CDATAL 0x94

static bool write_reg(uint8_t reg, uint8_t value) {
    uint8_t data[2] = {(uint8_t)(0x80 | reg), value};
    return i2c_write_blocking(i2c0, SENSOR_ADDR, data, 2, false) == 2;
}

static bool read_reg(uint8_t reg, uint8_t *value) {
    uint8_t address = (uint8_t)(0x80 | reg);
    if (i2c_write_blocking(i2c0, SENSOR_ADDR, &address, 1, true) != 1) return false;
    return i2c_read_blocking(i2c0, SENSOR_ADDR, value, 1, false) == 1;
}

static bool read_channels(uint16_t out[4]) {
    uint8_t address = (uint8_t)(0x80 | 0x20 | REG_CDATAL);
    uint8_t data[8];
    if (i2c_write_blocking(i2c0, SENSOR_ADDR, &address, 1, true) != 1) return false;
    if (i2c_read_blocking(i2c0, SENSOR_ADDR, data, sizeof data, false) != sizeof data) return false;
    for (unsigned i = 0; i < 4; ++i) out[i] = (uint16_t)data[2*i] | ((uint16_t)data[2*i+1] << 8);
    return true;
}

int main(void) {
    stdio_init_all();
    i2c_init(i2c0, 100000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);
    gpio_init(6);
    gpio_set_dir(6, GPIO_OUT);
    gpio_put(6, 1);
    sleep_ms(100);

    uint8_t id = 0;
    if (!read_reg(REG_ID, &id) || (id != 0x44 && id != 0x4D && id != 0x10)) {
        printf("{\"error\":\"sensor_not_found\",\"id\":%u}\n", id);
        while (true) tight_loop_contents();
    }
    if (!write_reg(REG_ATIME, 0xD5) || !write_reg(REG_CONTROL, 0x00) || !write_reg(REG_ENABLE, 0x03)) {
        printf("{\"error\":\"sensor_init_failed\"}\n");
        while (true) tight_loop_contents();
    }
    sleep_ms(10);
    while (true) {
        uint16_t c[4];
        if (read_channels(c)) printf("{\"clear\":%u,\"red\":%u,\"green\":%u,\"blue\":%u}\n", c[0], c[1], c[2], c[3]);
        else printf("{\"error\":\"i2c_read_failed\"}\n");
        sleep_ms(250);
    }
}
