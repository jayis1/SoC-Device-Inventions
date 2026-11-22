#include "platter_meter.h"
#include <math.h>

bool analyze_intervals(const unsigned *v, size_t n, result_t *out) {
    if (!v || !out || n < 8 || n > 32) return false;
    double sum = 0.0, sq = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (v[i] < 500 || v[i] > 7000) return false;
        sum += v[i]; sq += (double)v[i] * v[i];
    }
    double mean = sum / n, variance = sq / n - mean * mean;
    if (mean <= 0 || variance < -0.01) return false;
    if (variance < 0) variance = 0;
    out->rpm = 60000.0 / mean;
    out->interval_cv_pct = sqrt(variance) / mean * 100.0;
    out->samples = n;
    return isfinite(out->rpm) && isfinite(out->interval_cv_pct);
}
