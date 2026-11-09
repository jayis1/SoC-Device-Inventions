#ifndef LEAF_PRESS_H
#define LEAF_PRESS_H
#include <stdbool.h>
#include <stdint.h>
#define LP_CODE_MAX 31
#define LP_LINE_MAX 48
typedef struct { uint32_t started_ms; uint32_t last_ms; char code[LP_CODE_MAX+1]; bool active; } lp_session;
/* Returns false for malformed or oversized IDs. */
bool lp_start(lp_session *s, const char *code, uint32_t now_ms, char *out, unsigned cap);
/* Produces a bounded MARK or END JSON line; timestamps wrap-safe for intervals <2^31 ms. */
bool lp_event(lp_session *s, bool end, uint32_t now_ms, char *out, unsigned cap);
#endif
