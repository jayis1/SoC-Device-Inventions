#include <stdint.h>
#include <stdio.h>

/* Board adapter must implement these functions for the chosen CH32V003 board. */
extern int hx711_read(int32_t *raw);
extern uint16_t adc_read_displacement(void);
extern int button_tare_pressed(void);
extern int button_mark_pressed(void);
extern void uart_write(const char *line);

static int32_t zero;
static int have_zero;

void cord_gauge_step(void) {
    char line[96];
    static unsigned sequence;
    int32_t raw = 0;
    uint16_t displacement = adc_read_displacement();
    if (hx711_read(&raw) != 0) return;
    if (button_tare_pressed() || !have_zero) {
        zero = raw;
        have_zero = 1;
    }
    (void)button_mark_pressed(); /* host may treat MARK as specimen boundary */
    (void)snprintf(line, sizeof line, "{\"n\":%u,\"force_raw\":%ld,\"displacement_adc\":%u}\r\n",
                   sequence++, (long)(raw - zero), (unsigned)displacement);
    uart_write(line);
}
