#include "edge_tally.h"
#include <assert.h>
#include <stdint.h>
int main(void) {
    edge_tally_t s;
    edge_tally_init(&s, true, UINT32_MAX - 10u);
    edge_tally_sample(&s, false, UINT32_MAX - 5u);
    edge_tally_sample(&s, false, 20u);
    assert(s.count == 1u);
    edge_tally_sample(&s, true, 21u);
    edge_tally_sample(&s, true, 46u);
    assert(s.count == 1u);
    edge_tally_sample(&s, false, 47u);
    edge_tally_sample(&s, false, 72u);
    assert(s.count == 2u);
    return 0;
}
