/* Cargo Canary portable event core. Designed by jayis1. */
#ifndef CARGO_CANARY_H
#define CARGO_CANARY_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum { CC_EVENT_BYTES = 24 };
typedef enum { CC_OK = 0, CC_BAD_ARGUMENT = -1, CC_BAD_CRC = -2 } cc_status_t;
typedef struct { int16_t low_centi_c, high_centi_c; uint8_t open_samples, close_samples; } cc_limits_t;
typedef struct { uint8_t consecutive_bad, consecutive_good; bool excursion_open; } cc_excursion_t;
typedef struct { uint32_t sequence, utc_s, monotonic_s; int16_t temperature_centi_c; uint16_t shock_mg, battery_mv; uint8_t flags; } cc_event_t;
void cc_excursion_init(cc_excursion_t *state);
bool cc_excursion_update(cc_excursion_t *state, const cc_limits_t *limits, int16_t temperature_centi_c, bool *opened, bool *closed);
uint16_t cc_crc16_ccitt(const uint8_t *data, size_t length);
cc_status_t cc_encode_event(const cc_event_t *event, uint8_t out[CC_EVENT_BYTES]);
cc_status_t cc_decode_event(const uint8_t in[CC_EVENT_BYTES], cc_event_t *event);
#endif
