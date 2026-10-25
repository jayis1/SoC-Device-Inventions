#include <stdbool.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "tusb.h"

#define PEDAL_PREV 14u
#define PEDAL_NEXT 15u
#define DEBOUNCE_MS 25u

static bool stable[2] = {true, true};
static bool candidate[2] = {true, true};
static absolute_time_t changed_at[2];

static void send_page_key(uint8_t keycode) {
    if (!tud_hid_ready()) return;
    uint8_t keys[6] = {keycode, 0, 0, 0, 0, 0};
    (void)tud_hid_keyboard_report(0, 1, keys);
    sleep_ms(8);
    (void)tud_hid_keyboard_report(0, 1, NULL);
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize) {
    (void)instance; (void)report_id; (void)report_type;
    (void)buffer; (void)bufsize;
}

int main(void) {
    stdio_init_all();
    gpio_init(PEDAL_PREV); gpio_set_dir(PEDAL_PREV, GPIO_IN); gpio_pull_up(PEDAL_PREV);
    gpio_init(PEDAL_NEXT); gpio_set_dir(PEDAL_NEXT, GPIO_IN); gpio_pull_up(PEDAL_NEXT);
    gpio_init(PICO_DEFAULT_LED_PIN); gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    absolute_time_t now = get_absolute_time();
    changed_at[0] = changed_at[1] = now;

    while (true) {
        tud_task();
        const uint gpio[2] = {PEDAL_PREV, PEDAL_NEXT};
        for (unsigned i = 0; i < 2; ++i) {
            bool raw = gpio_get(gpio[i]);
            if (raw != candidate[i]) { candidate[i] = raw; changed_at[i] = get_absolute_time(); }
            if (candidate[i] != stable[i] && absolute_time_diff_us(changed_at[i], get_absolute_time()) >= DEBOUNCE_MS * 1000) {
                stable[i] = candidate[i];
                if (!stable[i]) send_page_key(i == 0 ? HID_KEY_PAGE_UP : HID_KEY_PAGE_DOWN);
            }
        }
        gpio_put(PICO_DEFAULT_LED_PIN, tud_mounted());
        sleep_ms(1);
    }
}
