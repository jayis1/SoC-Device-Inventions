#include <stdio.h>
#include <math.h>

void shade_scout_print_record(float angle_deg, float lux) {
    if (!isfinite(angle_deg) || !isfinite(lux)) puts("nan,nan");
    else printf("%.1f,%.1f\n", angle_deg, lux);
}
#ifdef SHADE_SCOUT_TEST
int main(void) {
    puts("angle_deg,lux");
    shade_scout_print_record(90.0f, 123.4f);
    shade_scout_print_record(NAN, NAN);
    return 0;
}
#endif
