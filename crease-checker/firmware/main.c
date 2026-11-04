#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "core.h"
#define HX_DOUT 2
#define HX_SCK 3
#define SDA_PIN 4
#define SCL_PIN 5
#define BUTTON 6
static bool capturing;
static int32_t hx_read(void) {
    uint32_t start = to_ms_since_boot(get_absolute_time());
    while (gpio_get(HX_DOUT)) {
        if (to_ms_since_boot(get_absolute_time()) - start > 100) return 0;
        tight_loop_contents();
    }
    uint32_t value = 0;
    for (unsigned i = 0; i < 24; ++i) {
        gpio_put(HX_SCK, 1); busy_wait_us_32(1);
        value = (value << 1) | (uint32_t)gpio_get(HX_DOUT);
        gpio_put(HX_SCK, 0); busy_wait_us_32(1);
    }
    gpio_put(HX_SCK, 1); busy_wait_us_32(1); gpio_put(HX_SCK, 0);
    if (value & 0x800000u) value |= 0xff000000u;
    return (int32_t)value;
}
int main(void) {
    stdio_init_all();
    gpio_init(HX_DOUT); gpio_set_dir(HX_DOUT, GPIO_IN); gpio_pull_up(HX_DOUT);
    gpio_init(HX_SCK); gpio_set_dir(HX_SCK, GPIO_OUT); gpio_put(HX_SCK, 0);
    gpio_init(BUTTON); gpio_set_dir(BUTTON, GPIO_IN); gpio_pull_up(BUTTON);
    i2c_init(i2c0, 100000); gpio_set_function(SDA_PIN, GPIO_FUNC_I2C); gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN); gpio_pull_up(SCL_PIN);
    printf("millis,raw_force,angle_raw\n");
    bool prev = true;
    while (true) {
        bool now = gpio_get(BUTTON);
        if (!now && prev) { sleep_ms(30); if (!gpio_get(BUTTON)) { capturing = !capturing; printf("#%s\n", capturing ? "START" : "END"); } }
        prev = now;
        if (capturing) {
            uint8_t reg = 0x0E, angle[2] = {0};
            int rc = i2c_write_blocking(i2c0, 0x36, &reg, 1, true);
            if (rc == 1 && i2c_read_blocking(i2c0, 0x36, angle, 2, false) == 2)
                csv_emit(stdout, to_ms_since_boot(get_absolute_time()), hx_read(), (uint16_t)(((angle[0] << 8) | angle[1]) & 0x0FFF));
            else printf("#SENSOR_ERROR\n");
            sleep_ms(20);
        } else sleep_ms(10);
    }
}
