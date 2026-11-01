#include "hinge_sag.h"
#include <assert.h>
#include <math.h>
int main(void) {
  assert(hs_angle_mdeg(0,0,1)==0);
  assert(hs_angle_mdeg(1,0,0)>89900 && hs_angle_mdeg(1,0,0)<90100);
  assert(hs_angle_mdeg(NAN,0,1)==0);
  hs_capture_t c; hs_capture_init(&c);
  assert(hs_capture_add(&c,100,2)); assert(hs_capture_add(&c,-50,2));
  assert(!hs_capture_add(&c,900,2));
  assert(c.count==2 && c.first_mdeg==100 && c.min_mdeg==-50 && c.max_mdeg==100);
  return 0;
}
