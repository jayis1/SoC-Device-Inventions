#include "core.h"
void scout_init(scout_t *s) { s->events=0; s->test=0; s->last_ms=0; s->active=0; s->armed=1; }
void scout_start(scout_t *s, uint32_t now_ms) { s->test++; s->events=0; s->last_ms=now_ms; s->active=1; s->armed=1; }
int scout_sample(scout_t *s, uint16_t adc, uint16_t threshold, uint32_t now_ms) {
 if (adc < threshold) { s->armed=1; return 0; }
 if (!s->armed) return 0;
 s->armed=0;
 if (!s->active) return 0;
 /* Unsigned subtraction remains valid over millis wrap. */
 if ((uint32_t)(now_ms-s->last_ms)<20u) return 0;
 s->last_ms=now_ms; s->events++; return 1;
}
