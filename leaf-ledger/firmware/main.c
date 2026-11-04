#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"

#define X_A 2
#define X_B 3
#define Y_A 4
#define Y_B 5
#define BUTTON 6
#define DEBOUNCE_US 1000
#define MAX_POINTS 2048

typedef struct { int32_t x, y; } point_t;
static point_t points[MAX_POINTS];
static unsigned count;
static int32_t x, y;
static uint8_t previous;
static const int8_t quad[16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};

static uint8_t read_ab(uint pin_a, uint pin_b) {
    return (uint8_t)((gpio_get(pin_a) << 1) | gpio_get(pin_b));
}

int main(void) {
    stdio_init_all();
    const uint pins[] = {X_A, X_B, Y_A, Y_B, BUTTON};
    for (unsigned i = 0; i < 5; ++i) { gpio_init(pins[i]); gpio_set_dir(pins[i], GPIO_IN); gpio_pull_up(pins[i]); }
    previous = (uint8_t)((read_ab(X_A,X_B) << 2) | read_ab(Y_A,Y_B));
    puts("{\"event\":\"ready\",\"units\":\"encoder_counts\"}");
    uint64_t last_sample = time_us_64();
    bool was_down = false, tracing = false;
    while (true) {
        uint64_t now = time_us_64();
        if (now - last_sample >= DEBOUNCE_US) {
            last_sample = now;
            uint8_t current = (uint8_t)((read_ab(X_A,X_B) << 2) | read_ab(Y_A,Y_B));
            x += quad[((previous & 3) << 2) | (current & 3)];
            y += quad[(((previous >> 2) & 3) << 2) | ((current >> 2) & 3)];
            previous = current;
            bool down = !gpio_get(BUTTON);
            if (down && !was_down) {
                tracing = !tracing;
                if (tracing) { count = 0; printf("{\"event\":\"trace_start\",\"x\":%ld,\"y\":%ld}\n", (long)x, (long)y); }
                else {
                    int64_t twice_area = 0;
                    if (count >= 3) for (unsigned i=0; i<count; ++i) { unsigned j=(i+1)%count; twice_area += (int64_t)points[i].x*points[j].y-(int64_t)points[j].x*points[i].y; }
                    printf("{\"event\":\"trace_end\",\"points\":%u,\"area_counts2\":%lld,\"closed\":true}\n",count,(long long)(twice_area<0?-twice_area:twice_area));
                }
                was_down = true;
            } else if (!down) was_down = false;
            if (tracing && count < MAX_POINTS && (count == 0 || points[count-1].x != x || points[count-1].y != y)) points[count++] = (point_t){x,y};
            if (tracing && count == MAX_POINTS) { puts("{\"event\":\"overflow\",\"trace_aborted\":true}"); tracing=false; }
        }
        tight_loop_contents();
    }
}
