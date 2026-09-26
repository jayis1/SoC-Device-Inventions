/* Cargo Canary host demonstration adapter. Designed by jayis1. */
#include "cargo_canary.h"
#include <stdio.h>
int main(void) { cc_event_t e={1,1700000000,300,-75,9200,3620,0x05}; uint8_t b[CC_EVENT_BYTES]; if(cc_encode_event(&e,b))return 1; printf("event sequence=%u temp=%.2fC crc=%02x%02x\n",e.sequence,e.temperature_centi_c/100.0,b[23],b[22]); return 0; }
