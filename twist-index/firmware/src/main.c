#include "pico/stdlib.h"
#include "core.h"
#include <stdio.h>

#define GATE_PIN 15
#define BUTTON_PIN 14
#define LED_PIN 25
static volatile uint32_t edges;
static volatile uint32_t last_edge_us;
static void gate_irq(uint gpio, uint32_t events) {
    if (gpio != GATE_PIN || !(events & GPIO_IRQ_EDGE_RISE)) return;
    uint32_t now = time_us_32();
    if ((uint32_t)(now - last_edge_us) >= 2000U) { edges++; last_edge_us = now; }
}
int main(void) {
    stdio_init_all();
    gpio_init(GATE_PIN); gpio_set_dir(GATE_PIN, GPIO_IN); gpio_pull_down(GATE_PIN);
    gpio_set_irq_enabled_with_callback(GATE_PIN, GPIO_IRQ_EDGE_RISE, true, gate_irq);
    gpio_init(BUTTON_PIN); gpio_set_dir(BUTTON_PIN, GPIO_IN); gpio_pull_up(BUTTON_PIN);
    gpio_init(LED_PIN); gpio_set_dir(LED_PIN, GPIO_OUT);
    uint32_t length = 0; bool running = false; char line[32]; unsigned pos = 0;
    printf("Twist Index; commands: L <mm>, S, E\n");
    while (true) {
        int ch = getchar_timeout_us(0);
        if (ch >= 0 && ch != '\r') {
            if (ch == '\n') {
                line[pos] = '\0';
                if (line[0] == 'L' && line[1] == ' ') { unsigned v=0; if (sscanf(line+2,"%u",&v)==1 && valid_length_mm(v)) length=v; else printf("ERR,length\n"); }
                else if (line[0]=='S' && length && !running) { edges=0; last_edge_us=0; running=true; gpio_put(LED_PIN,1); printf("CSV,turns,length_mm,turns_per_km\n"); }
                else if (line[0]=='E' && running) { running=false; gpio_put(LED_PIN,0); printf("DATA,%lu,%lu,%lu\n",(unsigned long)edges,(unsigned long)length,(unsigned long)turns_per_km(edges,length)); }
                else printf("ERR,command_or_state\n"); pos=0;
            } else if (pos < sizeof line - 1) line[pos++]=(char)ch; else pos=0;
        }
        if (running && !gpio_get(BUTTON_PIN)) { sleep_ms(30); if (!gpio_get(BUTTON_PIN)) { running=false; gpio_put(LED_PIN,0); printf("DATA,%lu,%lu,%lu\n",(unsigned long)edges,(unsigned long)length,(unsigned long)turns_per_km(edges,length)); while(!gpio_get(BUTTON_PIN)) sleep_ms(10); } }
        tight_loop_contents();
    }
}
