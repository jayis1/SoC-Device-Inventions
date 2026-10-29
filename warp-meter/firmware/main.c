#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define ENC_A 2
#define ENC_B 3
#define YARN_GATE 14
#define ZERO_BUTTON 15
#define LED_PIN 25
#define COUNTS_PER_TURN 24
#define LENGTH_MM 100

static volatile int32_t encoder_counts;
static volatile uint8_t previous_ab;

static void encoder_irq(uint gpio, uint32_t events) {
    (void)gpio; (void)events;
    const uint8_t ab = (uint8_t)((gpio_get(ENC_A) << 1) | gpio_get(ENC_B));
    static const int8_t delta[16] = {0,-1,1,0, 1,0,0,-1, -1,0,0,1, 0,1,-1,0};
    encoder_counts += delta[(previous_ab << 2) | ab];
    previous_ab = ab;
}

int main(void) {
    stdio_init_all();
    gpio_init(ENC_A); gpio_set_dir(ENC_A, GPIO_IN); gpio_pull_up(ENC_A);
    gpio_init(ENC_B); gpio_set_dir(ENC_B, GPIO_IN); gpio_pull_up(ENC_B);
    gpio_init(YARN_GATE); gpio_set_dir(YARN_GATE, GPIO_IN); gpio_pull_up(YARN_GATE);
    gpio_init(ZERO_BUTTON); gpio_set_dir(ZERO_BUTTON, GPIO_IN); gpio_pull_up(ZERO_BUTTON);
    gpio_init(LED_PIN); gpio_set_dir(LED_PIN, GPIO_OUT);
    previous_ab = (uint8_t)((gpio_get(ENC_A) << 1) | gpio_get(ENC_B));
    gpio_set_irq_enabled_with_callback(ENC_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_irq);
    gpio_set_irq_enabled(ENC_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    while (true) {
        if (!gpio_get(ZERO_BUTTON)) {
            encoder_counts = 0;
            sleep_ms(30);
            while (!gpio_get(ZERO_BUTTON)) tight_loop_contents();
        }
        const int32_t count = encoder_counts;
        const bool yarn_present = gpio_get(YARN_GATE) == 0;
        gpio_put(LED_PIN, yarn_present);
        const float turns = (float)count / COUNTS_PER_TURN;
        const float turns_per_m = turns * 1000.0f / LENGTH_MM;
        printf("{\"event\":\"sample\",\"counts\":%ld,\"turns\":%.3f,\"length_mm\":%d,\"turns_per_m\":%.3f,\"yarn_present\":%s}\n",
               (long)count, turns, LENGTH_MM, turns_per_m, yarn_present ? "true" : "false");
        sleep_ms(250);
    }
}
