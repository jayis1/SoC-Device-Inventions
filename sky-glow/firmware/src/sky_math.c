#include "sky_math.h"
#include <math.h>
bool sky_mag_delta(double lux, double ref, double *out) {
    if (!out || !isfinite(lux) || !isfinite(ref) || lux <= 0.0 || ref <= 0.0) return false;
    *out = -2.5 * log10(lux / ref);
    return isfinite(*out);
}
