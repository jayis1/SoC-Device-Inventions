#include "measurement.h"
#include <assert.h>
int main(void) {
    int32_t b[BC_DOT_COUNT] = {0,10,20,30,40,50};
    int32_t t[BC_DOT_COUNT] = {100,120,140,160,180,200};
    struct bc_result r;
    assert(bc_measure(b,t,&r)==0);
    assert(r.height_um[0]==100 && r.height_um[5]==150);
    assert(r.min_um==100 && r.max_um==150 && r.mean_um==125);
    assert(bc_measure(0,t,&r)==-1);
    return 0;
}
