/* Cargo Canary portable event core. Designed by jayis1. */
#include "cargo_canary.h"
#include <string.h>
static void put16(uint8_t *p, uint16_t x) { p[0]=(uint8_t)x; p[1]=(uint8_t)(x>>8); }
static void put32(uint8_t *p, uint32_t x) { for (unsigned i=0;i<4;i++) p[i]=(uint8_t)(x>>(8*i)); }
static uint16_t get16(const uint8_t *p) { return (uint16_t)p[0] | ((uint16_t)p[1]<<8); }
static uint32_t get32(const uint8_t *p) { uint32_t x=0; for(unsigned i=0;i<4;i++) x|=(uint32_t)p[i]<<(8*i); return x; }
void cc_excursion_init(cc_excursion_t *s) { if(s) memset(s,0,sizeof(*s)); }
bool cc_excursion_update(cc_excursion_t *s,const cc_limits_t *l,int16_t t,bool *opened,bool *closed) {
 if(!s||!l||!l->open_samples||!l->close_samples) return false; if(opened)*opened=false; if(closed)*closed=false;
 bool bad=t<l->low_centi_c||t>l->high_centi_c;
 if(bad){s->consecutive_good=0;if(s->consecutive_bad<255)s->consecutive_bad++;if(!s->excursion_open&&s->consecutive_bad>=l->open_samples){s->excursion_open=true;if(opened)*opened=true;}}
 else {s->consecutive_bad=0;if(s->consecutive_good<255)s->consecutive_good++;if(s->excursion_open&&s->consecutive_good>=l->close_samples){s->excursion_open=false;if(closed)*closed=true;}}
 return s->excursion_open;
}
uint16_t cc_crc16_ccitt(const uint8_t *data,size_t n){uint16_t c=0xffff;if(!data)return 0;while(n--){c^=(uint16_t)*data++<<8;for(unsigned i=0;i<8;i++)c=(c&0x8000)?(uint16_t)((c<<1)^0x1021):(uint16_t)(c<<1);}return c;}
cc_status_t cc_encode_event(const cc_event_t *e,uint8_t out[CC_EVENT_BYTES]){if(!e||!out)return CC_BAD_ARGUMENT;memset(out,0,CC_EVENT_BYTES);out[0]='C';out[1]='C';out[2]=1;out[3]=e->flags;put32(out+4,e->sequence);put32(out+8,e->utc_s);put32(out+12,e->monotonic_s);put16(out+16,(uint16_t)e->temperature_centi_c);put16(out+18,e->shock_mg);put16(out+20,e->battery_mv);put16(out+22,cc_crc16_ccitt(out,22));return CC_OK;}
cc_status_t cc_decode_event(const uint8_t in[CC_EVENT_BYTES],cc_event_t *e){if(!in||!e)return CC_BAD_ARGUMENT;if(in[0]!='C'||in[1]!='C'||in[2]!=1||get16(in+22)!=cc_crc16_ccitt(in,22))return CC_BAD_CRC;e->flags=in[3];e->sequence=get32(in+4);e->utc_s=get32(in+8);e->monotonic_s=get32(in+12);e->temperature_centi_c=(int16_t)get16(in+16);e->shock_mg=get16(in+18);e->battery_mv=get16(in+20);return CC_OK;}
