#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define HX_DOUT 14
#define HX_SCK 15
#define TARE_BTN 10
#define MARK_BTN 11
#define OLED_SDA 4
#define OLED_SCL 5
#define HX_TIMEOUT_US 100000
#define SCALE_COUNTS_PER_GRAM 1000.0f /* replace with two-point calibration */
#define EMA_ALPHA 0.20f

static bool hx_read(int32_t *out) {
    absolute_time_t end = make_timeout_time_us(HX_TIMEOUT_US);
    while (gpio_get(HX_DOUT)) {
        if (time_reached(end)) return false;
        tight_loop_contents();
    }
    uint32_t v = 0;
    for (int i = 0; i < 24; ++i) {
        gpio_put(HX_SCK, 1); busy_wait_us_32(1);
        v = (v << 1) | gpio_get(HX_DOUT);
        gpio_put(HX_SCK, 0); busy_wait_us_32(1);
    }
    gpio_put(HX_SCK, 1); busy_wait_us_32(1); /* 25th pulse: gain 128 */
    gpio_put(HX_SCK, 0);
    *out = (v & 0x800000u) ? (int32_t)(v | 0xff000000u) : (int32_t)v;
    return true;
}

int main(void) {
    stdio_init_all();
    gpio_init(HX_DOUT); gpio_set_dir(HX_DOUT, GPIO_IN); gpio_pull_up(HX_DOUT);
    gpio_init(HX_SCK); gpio_set_dir(HX_SCK, GPIO_OUT); gpio_put(HX_SCK, 0);
    gpio_init(TARE_BTN); gpio_set_dir(TARE_BTN, GPIO_IN); gpio_pull_up(TARE_BTN);
    gpio_init(MARK_BTN); gpio_set_dir(MARK_BTN, GPIO_IN); gpio_pull_up(MARK_BTN);
    i2c_init(i2c0, 400000);
    gpio_set_function(OLED_SDA, GPIO_FUNC_I2C); gpio_set_function(OLED_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA); gpio_pull_up(OLED_SCL);
    int32_t raw; float zero = 0, filtered = 0, previous = 0;
    uint64_t start = time_us_64(), last = start; unsigned stage = 0;
    bool initialized = false;
    while (true) {
        if (!hx_read(&raw)) {
            printf("%llu,NaN,NaN,SENSOR_ERROR\n", (unsigned long long)((time_us_64()-start)/1000));
            sleep_ms(200); continue;
        }
        if (!initialized) { zero = (float)raw; filtered = 0; previous = 0; initialized = true; }
        if (!gpio_get(TARE_BTN)) { zero = (float)raw; filtered = 0; previous = 0; sleep_ms(250); }
        if (!gpio_get(MARK_BTN)) { ++stage; printf("%llu,%.2f,0.00,STAGE_%u\n", (unsigned long long)((time_us_64()-start)/1000), filtered, stage); sleep_ms(250); }
        float mass = ((float)raw-zero)/SCALE_COUNTS_PER_GRAM;
        filtered += EMA_ALPHA * (mass-filtered);
        uint64_t now=time_us_64(); float dt=(float)(now-last)/1000000.0f;
        float flow=(dt>0.05f && dt<2.0f)?(filtered-previous)/dt:0;
        printf("%llu,%.2f,%.2f,SAMPLE\n", (unsigned long long)((now-start)/1000), filtered, flow);
        previous=filtered; last=now; sleep_ms(200);
    }
}
