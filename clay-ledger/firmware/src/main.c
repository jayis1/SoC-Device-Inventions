#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "config.h"

static bool read_reg(uint8_t addr, uint8_t reg, uint8_t *dst, size_t n) {
    if (i2c_write_blocking(I2C_PORT, addr, &reg, 1, true) != 1) return false;
    return i2c_read_blocking(I2C_PORT, addr, dst, n, false) == (int)n;
}
static bool read_angle(uint16_t *raw) {
    uint8_t b[2];
    if (!read_reg(SENSOR_AS5600, 0x0C, b, 2)) return false;
    *raw = (uint16_t)(((b[0] & 0x0F) << 8) | b[1]);
    return true;
}
static bool read_environment(float *temp_c, float *rh) {
    const uint8_t cmd[2] = {0x24, 0x00};
    uint8_t b[6];
    if (i2c_write_blocking(I2C_PORT, SENSOR_SHT31, cmd, 2, false) != 2) return false;
    sleep_ms(20);
    if (i2c_read_blocking(I2C_PORT, SENSOR_SHT31, b, 6, false) != 6) return false;
    uint16_t tr = (uint16_t)(b[0] << 8 | b[1]);
    uint16_t hr = (uint16_t)(b[3] << 8 | b[4]);
    *temp_c = -45.0f + 175.0f * tr / 65535.0f;
    *rh = 100.0f * hr / 65535.0f;
    return true;
}
int main(void) {
    stdio_init_all();
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(PIN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PIN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PIN_SDA);
    gpio_pull_up(PIN_SCL);
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    sleep_ms(1500);
    printf("# Clay Ledger; relative fixture displacement; no calibrated accuracy implied\n");
    printf("# t_ms,angle_raw,position_mm,temp_C,rh_pct,status\n");
    uint16_t zero = 0, previous = 0;
    bool zero_valid = read_angle(&zero);
    previous = zero;
    absolute_time_t next = get_absolute_time();
    while (true) {
        int c = getchar_timeout_us(0);
        if (c == 'h') printf("# commands: z=zero current position, h=help\n");
        if (c == 'z') {
            if (read_angle(&zero)) { previous = zero; zero_valid = true; printf("# zero set\n"); }
            else printf("# ERR zero: AS5600 unavailable\n");
        }
        next = delayed_by_ms(next, SAMPLE_INTERVAL_MS);
        sleep_until(next);
        uint16_t raw;
        float t, rh;
        bool aok = read_angle(&raw);
        bool eok = read_environment(&t, &rh);
        if (aok && !zero_valid) { zero = raw; previous = raw; zero_valid = true; }
        if (aok) {
            int delta = (int)raw - (int)zero;
            if (delta > 2047) delta -= 4096;
            if (delta < -2048) delta += 4096;
            float mm = (delta / 4096.0f) * PULLEY_CIRCUMFERENCE_MM;
            gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));
            if (eok) printf("%lld,%u,%.3f,%.2f,%.2f,OK\n", to_ms_since_boot(get_absolute_time()), raw, mm, t, rh);
            else printf("%lld,%u,%.3f,,,ERR_SHT31\n", to_ms_since_boot(get_absolute_time()), raw, mm);
            previous = raw;
        } else printf("%lld,,,,,ERR_AS5600\n", to_ms_since_boot(get_absolute_time()));
        (void)previous;
        fflush(stdout);
    }
}
