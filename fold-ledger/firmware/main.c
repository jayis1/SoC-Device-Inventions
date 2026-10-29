#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#ifdef FOLD_LEDGER_TEST
int main(void) {
    bool stable = false, candidate = false; uint32_t changed = 0; unsigned emits = 0;
    const bool raw[] = {true,true,true,false,false,false};
    const uint32_t t[] = {10,14,15,20,24,25};
    for (unsigned i=0; i<6; ++i) {
        if(raw[i]!=candidate){candidate=raw[i];changed=t[i];}
        if(candidate!=stable && (uint32_t)(t[i]-changed)>=5u){stable=candidate; if(stable)++emits;}
    }
    return (emits==1 && !stable)?0:1;
}
#else
#include "pico/stdlib.h"
#define PIN_FOOT 14u
#define PIN_OPTICAL 15u
#define PIN_LED 16u
struct db { bool stable,candidate; uint32_t changed; };
static bool update(struct db *d,bool raw,uint32_t now){if(raw!=d->candidate){d->candidate=raw;d->changed=now;}if(d->candidate!=d->stable&&(uint32_t)(now-d->changed)>=5u){d->stable=d->candidate;return d->stable;}return false;}
static void event(uint32_t ms,const char *type){printf("{\"ms\":%lu,\"type\":\"%s\"}\n",(unsigned long)ms,type);}
int main(void){stdio_init_all();gpio_init(PIN_FOOT);gpio_set_dir(PIN_FOOT,GPIO_IN);gpio_pull_up(PIN_FOOT);gpio_init(PIN_OPTICAL);gpio_set_dir(PIN_OPTICAL,GPIO_IN);gpio_pull_up(PIN_OPTICAL);gpio_init(PIN_LED);gpio_set_dir(PIN_LED,GPIO_OUT);struct db f={false,false,0},o={false,false,0};uint32_t down=0;bool long_sent=false;while(true){uint32_t now=to_ms_since_boot(get_absolute_time());bool fr=!gpio_get(PIN_FOOT),orr=!gpio_get(PIN_OPTICAL);if(update(&f,fr,now)){if(f.stable){down=now;long_sent=false;}else if(!long_sent)event(now,"foot");}if(f.stable&&!long_sent&&(uint32_t)(now-down)>=2000u){event(now,"batch_start");long_sent=true;}if(update(&o,orr,now)&&o.stable)event(now,"optical");gpio_put(PIN_LED,f.stable||o.stable);sleep_ms(1);}}
#endif
