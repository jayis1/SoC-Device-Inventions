#include "../src/hinge_cycle.h"
#include <assert.h>
int main(void) {
 hinge_cycle_event_t e;
 hinge_cycle_reset();
 assert(!hinge_cycle_update(1000, &e));
 assert(!hinge_cycle_update(1100, &e));
 assert(hinge_cycle_update(1050, &e)); assert(!e.opening);
 assert(hinge_cycle_update(1080, &e)); assert(e.opening && e.cycle == 1);
 assert(!hinge_cycle_update(4096, &e));
 assert(!hinge_cycle_update(100, 0));
 return 0;
}
