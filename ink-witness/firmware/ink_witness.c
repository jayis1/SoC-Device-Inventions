#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct { bool active; uint32_t start_ms, last_edge_ms; unsigned edges; } iw_session;
void iw_start(iw_session *s, uint32_t now) { s->active=true; s->start_ms=now; s->last_edge_ms=now; s->edges=0; puts("{\"type\":\"BEGIN\"}"); }
void iw_sample(iw_session *s, bool blocked, bool *previous, uint32_t now) {
    if (!s->active || blocked==*previous || (uint32_t)(now-s->last_edge_ms)<20) return;
    *previous=blocked; s->last_edge_ms=now; ++s->edges;
    printf("{\"type\":\"EDGE\",\"ms\":%u,\"blocked\":%s}\n", (unsigned)(now-s->start_ms), blocked?"true":"false");
}
void iw_stop(iw_session *s, uint32_t now) { if (!s->active) return; printf("{\"type\":\"END\",\"ms\":%u,\"edges\":%u}\n", (unsigned)(now-s->start_ms),s->edges); s->active=false; }
