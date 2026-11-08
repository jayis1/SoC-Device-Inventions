#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define ONE_WIRE 2u
#define SDA_PIN 4u
#define SCL_PIN 5u
#define BUTTON 15u

static bool ow_reset(void) {
    gpio_set_dir(ONE_WIRE, GPIO_OUT); gpio_put(ONE_WIRE, 0); sleep_us(480);
    gpio_set_dir(ONE_WIRE, GPIO_IN); sleep_us(70);
    bool present = !gpio_get(ONE_WIRE); sleep_us(410); return present;
}
static void ow_write_bit(bool b) {
    gpio_set_dir(ONE_WIRE, GPIO_OUT); gpio_put(ONE_WIRE, 0);
    if (b) { sleep_us(6); gpio_set_dir(ONE_WIRE, GPIO_IN); sleep_us(64); }
    else { sleep_us(60); gpio_set_dir(ONE_WIRE, GPIO_IN); sleep_us(10); }
}
static bool ow_read_bit(void) {
    gpio_set_dir(ONE_WIRE, GPIO_OUT); gpio_put(ONE_WIRE, 0); sleep_us(3);
    gpio_set_dir(ONE_WIRE, GPIO_IN); sleep_us(10); bool b = gpio_get(ONE_WIRE); sleep_us(53); return b;
}
static void ow_write(uint8_t v) { for (unsigned i=0;i<8;i++,v>>=1) ow_write_bit(v&1); }
static uint8_t ow_read(void) { uint8_t v=0; for(unsigned i=0;i<8;i++) if(ow_read_bit()) v|=(1u<<i); return v; }
static uint8_t crc8(const uint8_t *p, unsigned n) {
    uint8_t c=0; while(n--) { uint8_t x=*p++; for(int i=0;i<8;i++){ uint8_t m=(c^x)&1; c>>=1; if(m)c^=0x8c; x>>=1; } } return c;
}
static bool temperature(float *out) {
    uint8_t s[9];
    if(!ow_reset()) return false; ow_write(0xcc); ow_write(0x44); // broadcast convert
    sleep_ms(750); // 12-bit conversion maximum
    if(!ow_reset()) return false; ow_write(0xcc); ow_write(0xbe);
    for(unsigned i=0;i<9;i++) s[i]=ow_read();
    if(crc8(s,8)!=s[8]) return false;
    int16_t raw=(int16_t)((s[1]<<8)|s[0]); float t=raw/16.0f;
    if(t < -20.0f || t > 110.0f) return false;
    *out=t; return true;
}
int main(void) {
    stdio_init_all();
    gpio_init(ONE_WIRE); gpio_pull_up(ONE_WIRE); gpio_init(BUTTON);
    gpio_set_dir(BUTTON,GPIO_IN); gpio_pull_up(BUTTON);
    i2c_init(i2c0,100000); gpio_set_function(SDA_PIN,GPIO_FUNC_I2C); gpio_set_function(SCL_PIN,GPIO_FUNC_I2C);
    bool running=false, last=true; uint32_t start=0, last_sample=0;
    printf("elapsed_s,temp_c\r\n");
    while(true) {
        bool now=gpio_get(BUTTON);
        if(last && !now) { sleep_ms(25); if(!gpio_get(BUTTON)) { running=!running; if(running){start=to_ms_since_boot(get_absolute_time()); last_sample=0; printf("#run,start\r\n");} else printf("#run,stop\r\n"); } }
        last=now;
        uint32_t ms=to_ms_since_boot(get_absolute_time());
        if(running && ms-last_sample>=1000) {
            last_sample=ms; float t;
            if(temperature(&t)) printf("%lu,%.2f\r\n",(unsigned long)((ms-start)/1000),t);
            else printf("#sensor_error,%lu\r\n",(unsigned long)((ms-start)/1000));
            fflush(stdout);
        }
        sleep_ms(5);
    }
}
