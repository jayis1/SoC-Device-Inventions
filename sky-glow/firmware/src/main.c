#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#define SENSOR 0x29
#define CMD 0xA0
static bool read16(uint8_t reg, uint16_t *out) {
    uint8_t tx=CMD|reg, rx[2];
    if(i2c_write_timeout_us(i2c0,SENSOR,&tx,1,true,20000)!=1) return false;
    if(i2c_read_timeout_us(i2c0,SENSOR,rx,2,false,20000)!=2) return false;
    *out=(uint16_t)rx[0]|((uint16_t)rx[1]<<8); return true;
}
int main(void) {
    stdio_init_all(); i2c_init(i2c0,100000); gpio_set_function(4,GPIO_FUNC_I2C); gpio_set_function(5,GPIO_FUNC_I2C);
    gpio_pull_up(4); gpio_pull_up(5); gpio_init(PICO_DEFAULT_LED_PIN); gpio_set_dir(PICO_DEFAULT_LED_PIN,GPIO_OUT);
    sleep_ms(250); uint8_t init[]={CMD|0x00,0x03};
    bool online=i2c_write_timeout_us(i2c0,SENSOR,init,2,false,20000)==2;
    printf("status,sky_glow_v1\nms,ch0,ch1,status\n");
    while(true) {
        uint16_t a=0,b=0; bool ok=online&&read16(0x14,&a)&&read16(0x16,&b);
        bool sat=ok&&(a==0xffff||b==0xffff);
        printf("%u,%u,%u,%s\n",to_ms_since_boot(get_absolute_time()),a,b,!ok?"sensor_error":sat?"saturated":"ok");
        gpio_put(PICO_DEFAULT_LED_PIN,!ok||sat); sleep_ms(1000);
    }
}
