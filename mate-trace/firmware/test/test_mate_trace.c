#include "mate_trace.h"
#include <assert.h>
#include <limits.h>
int main(void) {
    mate_trace_t s;
    mate_trace_init(&s);
    assert(!mate_trace_sample(&s, 10));
    assert(!mate_trace_start(&s, 0));
    assert(mate_trace_start(&s, 1));
    assert(!mate_trace_start(&s, 2));
    assert(mate_trace_sample(&s, 12));
    assert(mate_trace_sample(&s, -19));
    assert(s.peak_counts == 19 && s.sample_count == 2);
    assert(!mate_trace_sample(&s, INT32_MIN));
    assert(mate_trace_finish(&s)); /* successful finish returns true */
    assert(!s.active && !mate_trace_finish(&s));
    assert(mate_trace_start(&s, 2));
    assert(!mate_trace_finish(&s)); /* empty run is rejected */
    return 0;
}
