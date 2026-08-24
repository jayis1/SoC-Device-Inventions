#include <math.h>

#include "psychrometrics.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double dr_compute_dew_point_c(double temp_c, double rh_pct)
{
    const double a = 17.62;
    const double b = 243.12;
    double rh = rh_pct / 100.0;

    if (rh < 0.01) {
        rh = 0.01;
    }
    if (rh > 1.0) {
        rh = 1.0;
    }

    double gamma = log(rh) + (a * temp_c) / (b + temp_c);
    return (b * gamma) / (a - gamma);
}

double dr_compute_condensation_margin_c(const dr_capture_t *capture, double dew_point_c)
{
    double coldest_surface = capture->frame_temp_c;

    if (capture->surface_temp_c < coldest_surface) {
        coldest_surface = capture->surface_temp_c;
    }

    return coldest_surface - dew_point_c;
}

double dr_compute_velocity_mps(double pressure_pa, double bridge_v)
{
    const double air_density = 1.204;
    const double dp_velocity = sqrt(fabs(2.0 * pressure_pa / air_density));
    const double kings_A = 0.10;
    const double kings_B = 2.90;
    const double kings_n = 0.52;
    double hotwire_velocity = 0.0;

    if (bridge_v > kings_A) {
        hotwire_velocity = pow((bridge_v - kings_A) / kings_B, 1.0 / kings_n);
    }

    return 0.45 * dp_velocity + 0.55 * hotwire_velocity;
}
