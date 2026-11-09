#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>

#define SDA_PIN 4
#define SCL_PIN 5
#define TCS_ADDR 0x29
#define CMD_REG 0x80

static bool sensor_read(uint8_t reg, uint8_t *buf, size_t n) {
    uint8_t cmd = (uint8_t)(CMD_REG | reg);
    return i2c_write_blocking(i2c0, TCS_ADDR, &cmd, 1, true) == 1 &&
           i2c_read_blocking(i2c0, TCS_ADDR, buf, n, false) == (int)n;
}

static void read_color(uint16_t v[4]) {
    uint8_t b[8];
    if (!sensor_read(0x14, b, sizeof b)) { printf("error,sensor_read\n"); return; }
    for (unsigned i=0;i<4;i++) v[i]=(uint16_t)(b[2*i] | ((uint16_t)b[2*i+1]<<8));
}

int main(void) {
    stdio_init_all();
    sleep_ms(1800);
    i2c_init(i2c0, 100000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C); gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN); gpio_pull_up(SCL_PIN);
    uint8_t id=0;
    if (!sensor_read(0x12, &id, 1) || (id != 0x44 && id != 0x4D && id != 0x10)) {
        printf("error,sensor_missing,id=%02x\n", id);
    } else {
        const uint8_t enable[]={CMD_REG|0x00,0x03};
        const uint8_t timing[]={CMD_REG|0x01,0xD5};
        i2c_write_blocking(i2c0,TCS_ADDR,enable,2,false);
        i2c_write_blocking(i2c0,TCS_ADDR,timing,2,false);
        printf("time_ms,event,label,value,clear,red,green,blue\n");
    }
    char line[96]; unsigned used=0; uint64_t start=time_us_64()/1000;
    while (true) {
        int ch=getchar_timeout_us(1000);
        if (ch==PICO_ERROR_TIMEOUT) continue;
        if (ch=='\r') continue;
        if (ch=='\n') {
            line[used]='\0'; char label[40]; float ml;
            if (sscanf(line,"sample,%39[A-Za-z0-9_-]",label)==1) {
                uint16_t c[4]; read_color(c);
                printf("%llu,sample,%s,,%u,%u,%u,%u\n",(unsigned long long)(time_us_64()/1000-start),label,c[0],c[1],c[2],c[3]);
            } else if (sscanf(line,"volume,%f",&ml)==1 && ml>=0.0f && ml<=100.0f) {
                printf("%llu,volume,,%.3f,,,,\n",(unsigned long long)(time_us_64()/1000-start),(double)ml);
            } else if (sscanf(line,"mark,%39[A-Za-z0-9_-]",label)==1) {
                printf("%llu,mark,%s,,,,,\n",(unsigned long long)(time_us_64()/1000-start),label);
            } else printf("error,invalid_command\n");
            used=0;
        } else if (ch>=32 && ch<127) {
            if (used+1<sizeof line) line[used++]=(char)ch;
            else { used=0; printf("error,line_too_long\n"); }
        }
    }
}
