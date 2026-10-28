#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define KEY_PIN 15
#define PIEZO_PIN 14
#define DOT_MIN_MS 25
#define DOT_MAX_MS 250
#define MAX_ELEMENTS 8

static const char *codes[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--..",
    "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."
};

static char decode(const char *s) {
    for (int i = 0; i < 36; ++i) {
        if (strcmp(s, codes[i]) == 0) return i < 26 ? (char)('A' + i) : (char)('0' + i - 26);
    }
    return '?';
}

int main(void) {
    stdio_init_all();
    gpio_init(KEY_PIN);
    gpio_set_dir(KEY_PIN, GPIO_IN);
    gpio_pull_up(KEY_PIN);
    gpio_set_function(PIEZO_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(PIEZO_PIN);
    pwm_set_wrap(slice, 62499);
    pwm_set_chan_level(slice, pwm_gpio_to_channel(PIEZO_PIN), 31249);
    pwm_set_enabled(slice, true);
    pwm_set_gpio_level(PIEZO_PIN, 0);

    absolute_time_t edge = get_absolute_time();
    bool pressed = false;
    char code[MAX_ELEMENTS + 1] = {0};
    unsigned used = 0;
    bool printed = false;

    while (true) {
        bool now_pressed = !gpio_get(KEY_PIN);
        absolute_time_t now = get_absolute_time();
        int64_t elapsed = absolute_time_diff_us(edge, now) / 1000;
        if (now_pressed != pressed && elapsed >= 12) {
            edge = now;
            pressed = now_pressed;
            printed = false;
            if (pressed) {
                gpio_put(PIEZO_PIN, 1);
            } else {
                gpio_put(PIEZO_PIN, 0);
                if (used < MAX_ELEMENTS) {
                    unsigned duration = elapsed < 0 ? 0u : (unsigned)elapsed;
                    code[used++] = (duration < 2u * 80u) ? '.' : '-';
                } else {
                    used = 0;
                    printf("?\n");
                }
            }
        }
        if (!pressed && used && !printed) {
            int64_t gap = absolute_time_diff_us(edge, now) / 1000;
            if (gap >= 7 * 80) {
                code[used] = '\0';
                putchar(decode(code));
                fflush(stdout);
                used = 0;
                printed = true;
            }
        }
        sleep_ms(1);
    }
}
