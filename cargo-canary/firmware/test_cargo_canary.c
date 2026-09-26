#include "cargo_canary.h"
#include <assert.h>
#include <string.h>
int main(void) {
  cc_limits_t l={200,800,3,2}; cc_excursion_t s; bool o,c; cc_excursion_init(&s);
  assert(!cc_excursion_update(&s,&l,850,&o,&c)&&!o); assert(!cc_excursion_update(&s,&l,850,&o,&c)&&!o);
  assert(cc_excursion_update(&s,&l,850,&o,&c)&&o&&!c); assert(cc_excursion_update(&s,&l,600,&o,&c)&&!c);
  assert(!cc_excursion_update(&s,&l,600,&o,&c)&&c);
  cc_event_t a={42,1700000000,900,-123,8100,3550,5},b; uint8_t bytes[CC_EVENT_BYTES];
  assert(cc_encode_event(&a,bytes)==CC_OK); assert(cc_decode_event(bytes,&b)==CC_OK);
  assert(a.sequence==b.sequence && a.utc_s==b.utc_s && a.monotonic_s==b.monotonic_s);
  assert(a.temperature_centi_c==b.temperature_centi_c && a.shock_mg==b.shock_mg && a.battery_mv==b.battery_mv && a.flags==b.flags);
  bytes[10]^=1; assert(cc_decode_event(bytes,&b)==CC_BAD_CRC); return 0;
}
