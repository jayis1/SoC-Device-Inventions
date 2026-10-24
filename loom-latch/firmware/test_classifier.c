#include <assert.h>
#include "classifier.h"
int main(void) {
    classifier_t c = {0}; uint32_t dt = 0; bool dir = false;
    assert(!classifier_edge(&c, true, 1000, &dt, &dir));
    assert(classifier_edge(&c, false, 2000, &dt, &dir));
    assert(dt == 1000 && dir);
    assert(!classifier_edge(&c, false, 3000, &dt, &dir));
    assert(!classifier_edge(&c, false, 4000, &dt, &dir)); /* same gate */
    assert(!classifier_edge(&c, true, 4000000, &dt, &dir)); /* expired */
    return 0;
}
