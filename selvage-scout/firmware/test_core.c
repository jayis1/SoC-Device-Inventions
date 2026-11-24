#include "core.h"
#include <assert.h>
int main(void) { scout_t s; scout_init(&s); scout_start(&s,10); assert(!scout_sample(&s,3000,2000,15)); assert(!scout_sample(&s,1000,2000,20)); assert(scout_sample(&s,3000,2000,35)); assert(!scout_sample(&s,3000,2000,70)); assert(!scout_sample(&s,1000,2000,80)); assert(scout_sample(&s,3000,2000,100)); assert(s.events==2); return 0; }
