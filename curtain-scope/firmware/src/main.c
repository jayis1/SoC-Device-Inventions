#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

static const uint8_t gates[5] = {2, 3, 4, 5, 6};
static volatile int64_t stamps[5];
static volatile uint8_t seen;
static volatile bool armed;

static void edge_cb(uint gpio, uint32_t events) {
    if (!armed || !(events & GPIO_IRQ_EDGE_FALL)) return;
    for (unsigned i = 0; i < 5; ++i) if (gates[i] == gpio) {
        uint8_t bit = (uint8_t)(1u << i);
        if (!(seen & bit)) { stamps[i] = time_us_64(); seen |= bit; }
    }
}

int main(void) {
    stdio_init_all();
    gpio_init(15); gpio_set_dir(15, GPIO_OUT);
    for (unsigned i=0; i<5; ++i) {
        gpio_init(gates[i]); gpio_set_dir(gates[i], GPIO_IN); gpio_pull_up(gates[i]);
        gpio_set_irq_enabled_with_callback(gates[i], GPIO_IRQ_EDGE_FALL, true, edge_cb);
    }
    char line[16]; size_t n=0;
    while (true) {
        int c=getchar_timeout_us(10000);
        if (c=='\n' || c=='\r') {
            line[n]='\0';
            if (n==5 && line[0]=='S' && line[1]=='T' && line[2]=='A' && line[3]=='R' && line[4]=='T') {
                armed=false; seen=0; for(unsigned i=0;i<5;++i) stamps[i]=0;
                armed=true; gpio_put(15,1);
                printf("index,us,status\n");
            }
            n=0;
        } else if(c>=0 && n<sizeof(line)-1) line[n++]=(char)c;
        if (armed && seen==0x1f) {
            armed=false; gpio_put(15,0);
            int64_t base=stamps[0];
            for(unsigned i=0;i<5;++i) printf("%u,%lld,ok\n",i,(long long)(stamps[i]-base));
            printf("END\n");
        }
        tight_loop_contents();
    }
}
