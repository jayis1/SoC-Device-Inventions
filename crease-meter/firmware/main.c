#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Host-testable core for RP2040 Pico SDK adapter. ADC input is a buffered
 * phototransistor divider; GPIO 15 is the optical gate. */
#define SAMPLE_COUNT 64u
#define ADC_MAX 4095u
static uint16_t median_u16(uint16_t *v, size_t n) {
    for (size_t i=1;i<n;i++){uint16_t x=v[i];size_t j=i;while(j&&v[j-1]>x){v[j]=v[j-1];j--;}v[j]=x;}
    return v[n/2];
}
/* Output normalized transmission in permille, or false for invalid baseline. */
bool crease_transmission(uint16_t baseline, uint16_t sample, uint16_t *permille) {
    if (!permille || baseline < 32 || sample > baseline) return false;
    *permille=(uint16_t)(((uint32_t)sample*1000u + baseline/2u)/baseline);
    return true;
}
#ifdef CREASE_METER_HOST_TEST
int main(void) {
 uint16_t result=0;
 if(!crease_transmission(2000,1000,&result)||result!=500) return 1;
 if(crease_transmission(0,0,&result)) return 2;
 if(crease_transmission(100,101,&result)) return 3;
 puts("crease-meter core tests: 3 passed"); return 0;
}
#endif
