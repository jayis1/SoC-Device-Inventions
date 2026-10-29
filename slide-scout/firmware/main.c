#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "density.h"

#define SDA_PIN 4
#define SCL_PIN 5
#define LED_PIN 12
#define SENSOR_ADDR 0x29

int main(void) {
    stdio_init_all();
    gpio_init(LED_PIN); gpio_set_dir(LED_PIN, GPIO_OUT); gpio_put(LED_PIN, 1);
    i2c_init(i2c0, 100000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C); gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN); gpio_pull_up(SCL_PIN);
    sleep_ms(1500);
    printf("slide_scout,1\nstatus,not_implemented\n");
    while (true) {
        uint8_t id_reg = 0x12, id = 0;
        int rc = i2c_write_blocking(i2c0, SENSOR_ADDR, &id_reg, 1, true);
        if (rc == 1) rc = i2c_read_blocking(i2c0, SENSOR_ADDR, &id, 1, false);
        if (rc == 1 && id == 0x50) printf("status,sensor_detected_register_reader_pending\n");
        else printf("status,sensor_error,rc=%d,id=%u\n", rc, id);
        sleep_ms(1000);
    }
}
