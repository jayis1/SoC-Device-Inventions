#include <math.h>
#include <stddef.h>

#include "dsp.h"

double ts_compute_rms(const double *samples, size_t count)
{
    if ((samples == NULL) || (count == 0U)) {
        return 0.0;
    }

    double sum_sq = 0.0;
    for (size_t i = 0; i < count; ++i) {
        sum_sq += samples[i] * samples[i];
    }
    return sqrt(sum_sq / (double) count);
}

double ts_compute_crest_factor(const double *samples, size_t count)
{
    double rms = ts_compute_rms(samples, count);
    if (rms <= 1e-9) {
        return 0.0;
    }

    double peak = 0.0;
    for (size_t i = 0; i < count; ++i) {
        double value = fabs(samples[i]);
        if (value > peak) {
            peak = value;
        }
    }
    return peak / rms;
}

double ts_compute_burstiness(const double *samples, size_t count)
{
    if ((samples == NULL) || (count == 0U)) {
        return 0.0;
    }

    double mean = 0.0;
    for (size_t i = 0; i < count; ++i) {
        mean += samples[i];
    }
    mean /= (double) count;
    if (mean <= 1e-9) {
        return 0.0;
    }

    double variance = 0.0;
    for (size_t i = 0; i < count; ++i) {
        double delta = samples[i] - mean;
        variance += delta * delta;
    }
    variance /= (double) count;
    return sqrt(variance) / mean;
}

double ts_compute_active_fraction(const double *samples, size_t count, double threshold)
{
    if ((samples == NULL) || (count == 0U)) {
        return 0.0;
    }

    size_t active = 0U;
    for (size_t i = 0; i < count; ++i) {
        if (samples[i] >= threshold) {
            ++active;
        }
    }
    return (double) active / (double) count;
}

double ts_estimate_cycle_rate_cpm(const double *samples, size_t count, double sample_period_s)
{
    if ((samples == NULL) || (count < 8U) || (sample_period_s <= 0.0)) {
        return 0.0;
    }

    double mean = 0.0;
    for (size_t i = 0; i < count; ++i) {
        mean += samples[i];
    }
    mean /= (double) count;

    double best_corr = -1e9;
    size_t best_lag = 0U;
    for (size_t lag = 4U; lag < (count / 2U); ++lag) {
        double corr = 0.0;
        for (size_t i = 0; (i + lag) < count; ++i) {
            corr += (samples[i] - mean) * (samples[i + lag] - mean);
        }
        if (corr > best_corr) {
            best_corr = corr;
            best_lag = lag;
        }
    }

    if (best_lag == 0U || best_corr <= 0.02) {
        return 0.0;
    }

    double cycle_period_s = (double) best_lag * sample_period_s;
    return 60.0 / cycle_period_s;
}
