// Copyright (c) jayis1. Inherits repository license.
#include "pico/stdlib.h"
#include "core.h"
#include <stdio.h>
#define HX_DOUT 2
#define HX_SCK 3
#define TARE_SAMPLES 32
// Replace with experimentally established raw counts per newton; zero disables force output.
#define COUNTS_PER_NEWTON 0.0
static bool hx_read(int32_t *out) {
    absolute_time_t deadline = make_timeout_time_ms(500);
    while (gpio_get(HX_DOUT)) {
        if (time_reached(deadline)) return false;
        tight_loop_contents();
    }
    uint32_t v=0;
    for (int i=0;i<24;i++) {
        gpio_put(HX_SCK, 1); busy_wait_us_32(1);
        v=(v<<1)|(uint32_t)gpio_get(HX_DOUT);
        gpio_put(HX_SCK, 0); busy_wait_us_32(1);
    }
    gpio_put(HX_SCK, 1); busy_wait_us_32(1); gpio_put(HX_SCK, 0);
    if (v & 0x800000u) v |= 0xff000000u;
    *out=(int32_t)v;
    return true;
}
int main(void) {
    stdio_init_all();
    gpio_init(HX_DOUT); gpio_set_dir(HX_DOUT, GPIO_IN); gpio_pull_up(HX_DOUT);
    gpio_init(HX_SCK); gpio_set_dir(HX_SCK, GPIO_OUT); gpio_put(HX_SCK, 0);
    sleep_ms(1500);
    kw_state_t state; kw_init(&state, COUNTS_PER_NEWTON);
    int32_t tare[TARE_SAMPLES];
    for (unsigned i=0;i<TARE_SAMPLES;i++) {
        if (!hx_read(&tare[i])) { printf("ERROR,hx711_timeout\n"); return 1; }
    }
    if (!kw_tare(&state,tare,TARE_SAMPLES)) { printf("ERROR,tare\n"); return 1; }
    printf("time_ms,raw,force_n,peak_n\n");
    while (true) {
        int32_t raw;
        if (!hx_read(&raw)) { printf("ERROR,hx711_timeout\n"); sleep_ms(1000); continue; }
        double force=0;
        bool valid=kw_update(&state,raw,&force);
        if (valid) printf("%lu,%ld,%.3f,%.3f\n",(unsigned long)to_ms_since_boot(get_absolute_time()),(long)raw,force,state.peak_newton);
        else printf("%lu,%ld,NA,NA\n",(unsigned long)to_ms_since_boot(get_absolute_time()),(long)raw);
    }
}
