#ifndef SKY_MATH_H
#define SKY_MATH_H
#include <stdbool.h>
/* Relative magnitude delta from a positive reference; rejects invalid inputs. */
bool sky_mag_delta(double lux, double reference_lux, double *delta);
#endif
