#include <assert.h>
#include <stdio.h>
#include "src/tape_tally.h"
int main(void) {
 tt_event e;
 assert(tt_sample(true, 1000, &e) && e.type == TT_PULSE);
 assert(!tt_sample(false, 1200, &e));
 assert(tt_sample(false, 1500, &e) && e.type == TT_STALL && e.value_ms == 500);
 assert(!tt_sample(false, 1600, &e));
 assert(tt_sample(true, 1700, &e) && e.type == TT_PULSE);
 puts("tape tally core tests passed");
 return 0;
}
