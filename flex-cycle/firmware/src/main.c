#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <inttypes.h>
#include <stdio.h>
#define HALL 2u
#define TOGGLE 14u
#define RESET 15u
static volatile uint32_t count, last_us;
static volatile bool active;
static uint32_t began, frozen;
static void irq(uint gpio, uint32_t events) {
    (void)gpio; (void)events;
    uint32_t now=time_us_32();
    if(active && (uint32_t)(now-last_us)>=100000u) {
        last_us=now;
        if(count!=UINT32_MAX) ++count;
    }
}
static bool low(uint p){return gpio_get(p)==0;}
static void release(uint p){sleep_ms(20);while(low(p))sleep_ms(2);}
static uint32_t elapsed(void){return active?(uint32_t)(time_us_32()-began)/1000u:frozen;}
int main(void){
    stdio_init_all();
    gpio_init(HALL);gpio_set_dir(HALL,GPIO_IN);gpio_pull_up(HALL);
    gpio_set_irq_enabled_with_callback(HALL,GPIO_IRQ_EDGE_FALL,true,irq);
    gpio_init(TOGGLE);gpio_set_dir(TOGGLE,GPIO_IN);gpio_pull_up(TOGGLE);
    gpio_init(RESET);gpio_set_dir(RESET,GPIO_IN);gpio_pull_up(RESET);
    bool prev_t=false,prev_r=false;
    for(;;){
        bool t=low(TOGGLE),r=low(RESET);
        if(t&&!prev_t){release(TOGGLE);
            if(!active){began=time_us_32();active=true;printf("{\"event\":\"start\",\"count\":%" PRIu32 "}\n",count);}
            else{frozen=elapsed();active=false;printf("{\"event\":\"stop\",\"count\":%" PRIu32 ",\"elapsed_ms\":%" PRIu32 "}\n",count,frozen);}
        }
        if(r&&!prev_r){release(RESET);active=false;count=0;frozen=0;printf("{\"event\":\"reset\",\"count\":0}\n");}
        prev_t=low(TOGGLE);prev_r=low(RESET);
        static uint32_t reported;
        uint32_t current=count;
        if(active && current!=reported){printf("{\"event\":\"cycle\",\"count\":%" PRIu32 ",\"elapsed_ms\":%" PRIu32 "}\n",current,elapsed());reported=current;}
        if(current==0)reported=0;
        tight_loop_contents();
    }
}
