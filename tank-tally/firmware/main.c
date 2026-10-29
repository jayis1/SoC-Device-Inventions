#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "pins.h"

static bool active;
static uint32_t started_ms, last_event_ms, count;
static float prev_mag;
static bool have_prev;

static bool lis3dh_init(void) {
    uint8_t reg[2] = {0x20, 0x57}; /* 100 Hz, XYZ enabled */
    if (i2c_write_blocking(i2c0, LIS_ADDR, reg, 2, false) < 0) return false;
    return true;
}
static bool read_magnitude(float *mag) {
    uint8_t ptr=0x28|0x80, b[6];
    if (i2c_write_blocking(i2c0,LIS_ADDR,&ptr,1,true)<0 || i2c_read_blocking(i2c0,LIS_ADDR,b,6,false)<0) return false;
    int32_t x=(int16_t)((b[1]<<8)|b[0]), y=(int16_t)((b[3]<<8)|b[2]), z=(int16_t)((b[5]<<8)|b[4]);
    *mag=(float)(x*x+y*y+z*z); /* relative squared magnitude; no floating sqrt */
    return true;
}
int main(void) {
    stdio_init_all();
    i2c_init(i2c0, 100000); gpio_set_function(PIN_SDA,GPIO_FUNC_I2C); gpio_set_function(PIN_SCL,GPIO_FUNC_I2C);
    gpio_pull_up(PIN_SDA); gpio_pull_up(PIN_SCL);
    gpio_init(PIN_BUTTON); gpio_set_dir(PIN_BUTTON,GPIO_IN); gpio_pull_up(PIN_BUTTON);
    sleep_ms(50);
    bool sensor_ok=lis3dh_init();
    while (true) {
        if (!gpio_get(PIN_BUTTON)) {
            sleep_ms(30);
            if (!gpio_get(PIN_BUTTON)) { while(!gpio_get(PIN_BUTTON)) sleep_ms(10);
                if (!active) { active=true; started_ms=to_ms_since_boot(get_absolute_time()); last_event_ms=started_ms; count=0; have_prev=false; printf("start\n"); }
                else { active=false; printf("end,%lu\n",(unsigned long)count); }
            }
        }
        if (active && sensor_ok) {
            float m; uint32_t now=to_ms_since_boot(get_absolute_time());
            if (read_magnitude(&m)) {
                if (have_prev && m > prev_mag*1.35f && now-last_event_ms>350) {
                    count++; printf("event,%lu,%lu,%lu\n",(unsigned long)count,(unsigned long)(now-started_ms),(unsigned long)(now-last_event_ms)); last_event_ms=now;
                }
                prev_mag=m; have_prev=true;
            }
        }
        tight_loop_contents(); sleep_ms(10);
    }
}
