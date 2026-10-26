#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#define I2C_PORT i2c0
#define PIN_SDA 4
#define PIN_SCL 5
#define HX_DOUT 10
#define HX_SCK 11
#define AS5600_ADDR 0x36
#define REG_RAW_ANGLE 0x0C
#define SAMPLE_MS 100

static bool read_angle(uint16_t *angle) {
    uint8_t reg = REG_RAW_ANGLE, data[2];
    if (i2c_write_blocking(I2C_PORT, AS5600_ADDR, &reg, 1, true) != 1) return false;
    if (i2c_read_blocking(I2C_PORT, AS5600_ADDR, data, 2, false) != 2) return false;
    *angle = (uint16_t)(((data[0] & 0x0F) << 8) | data[1]);
    return true;
}

static bool read_hx711(int32_t *raw) {
    absolute_time_t deadline = make_timeout_time_ms(100);
    while (gpio_get(HX_DOUT)) if (time_reached(deadline)) return false;
    uint32_t value = 0;
    for (int i = 0; i < 24; ++i) {
        gpio_put(HX_SCK, 1); busy_wait_us_32(1);
        value = (value << 1) | (uint32_t)gpio_get(HX_DOUT);
        gpio_put(HX_SCK, 0); busy_wait_us_32(1);
    }
    // 25th pulse selects channel A, gain 128 for next conversion.
    gpio_put(HX_SCK, 1); busy_wait_us_32(1); gpio_put(HX_SCK, 0);
    *raw = (value & 0x800000u) ? (int32_t)(value | 0xFF000000u) : (int32_t)value;
    return true;
}

int main(void) {
    stdio_init_all();
    i2c_init(I2C_PORT, 100000);
    gpio_set_function(PIN_SDA, GPIO_FUNC_I2C); gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_SDA); gpio_pull_up(PIN_SCL);
    gpio_init(HX_DOUT); gpio_set_dir(HX_DOUT, GPIO_IN);
    gpio_init(HX_SCK); gpio_set_dir(HX_SCK, GPIO_OUT); gpio_put(HX_SCK, 0);
    sleep_ms(1500);
    puts("sample_ms,angle_raw,force_raw,direction");
    uint16_t previous = 0; bool have_previous = false; int direction = 0;
    while (true) {
        uint16_t angle; int32_t force;
        bool a_ok = read_angle(&angle), f_ok = read_hx711(&force);
        if (!a_ok || !f_ok) {
            printf("%llu,INVALID,INVALID,%d\n", (unsigned long long)to_ms_since_boot(get_absolute_time()), direction);
        } else {
            if (have_previous) {
                int delta = (int)angle - (int)previous;
                if (delta > 2048) delta -= 4096;
                if (delta < -2048) delta += 4096;
                if (delta > 1) direction = 1;
                else if (delta < -1) direction = -1;
            }
            previous = angle; have_previous = true;
            printf("%llu,%u,%ld,%d\n", (unsigned long long)to_ms_since_boot(get_absolute_time()), angle, (long)force, direction);
        }
        sleep_ms(SAMPLE_MS);
    }
}
