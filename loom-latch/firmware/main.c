#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "classifier.h"
#define GATE_A 14
#define GATE_B 15
static classifier_t classifier;
static bool previous_a, previous_b;
int main(void) {
    stdio_init_all();
    gpio_init(GATE_A); gpio_set_dir(GATE_A, GPIO_IN); gpio_pull_up(GATE_A);
    gpio_init(GATE_B); gpio_set_dir(GATE_B, GPIO_IN); gpio_pull_up(GATE_B);
    previous_a = gpio_get(GATE_A); previous_b = gpio_get(GATE_B);
    while (true) {
        bool a = gpio_get(GATE_A), b = gpio_get(GATE_B);
        bool edge_a = a != previous_a, edge_b = b != previous_b;
        previous_a = a; previous_b = b;
        /* Falling edge means the beam is interrupted. Ignore clear edges. */
        if (edge_a && !a) {
            uint32_t duration; bool a_to_b;
            if (classifier_edge(&classifier, true, time_us_32(), &duration, &a_to_b))
                printf("{\"event\":\"pick\",\"direction\":\"%s\",\"duration_us\":%lu}\n", a_to_b ? "A>B" : "B>A", (unsigned long)duration);
        }
        if (edge_b && !b) {
            uint32_t duration; bool a_to_b;
            if (classifier_edge(&classifier, false, time_us_32(), &duration, &a_to_b))
                printf("{\"event\":\"pick\",\"direction\":\"%s\",\"duration_us\":%lu}\n", a_to_b ? "A>B" : "B>A", (unsigned long)duration);
        }
        sleep_us(100);
    }
}
