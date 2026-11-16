#include "pico/stdlib.h"
#include <math.h>
void shade_scout_print_record(float angle_deg, float lux);
int main(void) {
    stdio_init_all();
    sleep_ms(1500);
    puts("angle_deg,lux");
    while (true) {
        // Sensor drivers intentionally not yet implemented. Fail visibly.
        shade_scout_print_record(NAN, NAN);
        sleep_ms(1000);
    }
}
