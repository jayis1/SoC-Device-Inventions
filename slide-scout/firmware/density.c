#include <math.h>
#include <stdbool.h>
#include <stdint.h>

/* intensity units from sensor after dark-offset correction */
bool density_from_ratio(double reference, double sample, double *od) {
    if (!od || !isfinite(reference) || !isfinite(sample) || reference <= 0.0 || sample <= 0.0 || sample > reference) return false;
    *od = log10(reference / sample);
    return isfinite(*od);
}
