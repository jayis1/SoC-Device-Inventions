#include <stdbool.h>
#include <stdint.h>

#define CREASE_STOP_ADC_MAX 4095u

typedef struct {
    uint32_t sequence;
    uint16_t position_raw;
} crease_stop_event_t;

/* Return true only on a debounced press edge. Caller owns GPIO debounce timing. */
bool crease_stop_record(bool pressed, uint16_t adc_raw,
                        crease_stop_event_t *event);
