#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "board_config.h"

static const uint mux_pins[4] = { PIN_MUX_S0, PIN_MUX_S1, PIN_MUX_S2, PIN_MUX_S3 };
static float zero_deg = 0.0f, span_deg = 270.0f;
static bool calibrated = false;

static uint16_t sample_channel(unsigned ch) {
    for (unsigned b = 0; b < 4; ++b) gpio_put(mux_pins[b], (ch >> b) & 1u);
    sleep_us(80);
    return adc_read();
}

static float pointer_angle(void) {
    uint16_t values[SENSOR_COUNT];
    for (unsigned i = 0; i < SENSOR_COUNT; ++i) values[i] = sample_channel(i);
    unsigned darkest = 0;
    for (unsigned i = 1; i < SENSOR_COUNT; ++i) if (values[i] < values[darkest]) darkest = i;
    return (float)darkest * (360.0f / (float)SENSOR_COUNT);
}

int main(void) {
    stdio_init_all();
    adc_init(); adc_gpio_init(PIN_ADC_MUX); adc_select_input(0);
    for (unsigned i = 0; i < 4; ++i) { gpio_init(mux_pins[i]); gpio_set_dir(mux_pins[i], GPIO_OUT); }
    gpio_init(PIN_ZERO); gpio_set_dir(PIN_ZERO, GPIO_IN); gpio_pull_up(PIN_ZERO);
    gpio_init(PIN_SPAN); gpio_set_dir(PIN_SPAN, GPIO_IN); gpio_pull_up(PIN_SPAN);
    sleep_ms(1200);
    while (true) {
        float angle = pointer_angle();
        if (!gpio_get(PIN_ZERO)) { zero_deg = angle; calibrated = false; sleep_ms(250); }
        if (!gpio_get(PIN_SPAN)) { span_deg = angle - zero_deg; calibrated = fabsf(span_deg) > 1.0f; sleep_ms(250); }
        if (calibrated) {
            float fraction = (angle - zero_deg) / span_deg;
            printf("{\"status\":\"reading\",\"angle_deg\":%.1f,\"fraction\":%.4f,\"value\":%.2f}\n", angle, fraction, fraction * FULL_SCALE);
        } else printf("{\"status\":\"uncalibrated\",\"angle_deg\":%.1f}\n", angle);
        sleep_ms(200);
    }
}
