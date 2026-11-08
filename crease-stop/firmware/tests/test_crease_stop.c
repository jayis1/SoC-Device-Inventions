#include <assert.h>
#include "crease_stop.h"

int main(void)
{
    crease_stop_event_t event = {0u, 0u};
    assert(crease_stop_record(true, 1234u, &event));
    assert(event.sequence == 1u && event.position_raw == 1234u);
    assert(!crease_stop_record(true, 2345u, &event));
    assert(!crease_stop_record(false, 2345u, &event));
    assert(crease_stop_record(true, CREASE_STOP_ADC_MAX, &event));
    assert(event.sequence == 2u && event.position_raw == CREASE_STOP_ADC_MAX);
    assert(!crease_stop_record(true, CREASE_STOP_ADC_MAX + 1u, &event));
    assert(!crease_stop_record(true, 0u, 0));
    return 0;
}
