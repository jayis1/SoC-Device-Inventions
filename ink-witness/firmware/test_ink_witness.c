#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
typedef struct { bool active; uint32_t start_ms, last_edge_ms; unsigned edges; } iw_session;
void iw_start(iw_session *, uint32_t);
void iw_sample(iw_session *, bool, bool *, uint32_t);
void iw_stop(iw_session *, uint32_t);
int main(void) { iw_session s={0}; bool prev=false; iw_start(&s,100); iw_sample(&s,true,&prev,110); assert(s.edges==0); iw_sample(&s,true,&prev,125); assert(s.edges==1); iw_sample(&s,false,&prev,150); assert(s.edges==2); iw_stop(&s,200); assert(!s.active && s.edges==2); return 0; }
