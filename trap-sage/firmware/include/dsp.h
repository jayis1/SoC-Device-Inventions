#ifndef DSP_H
#define DSP_H

#include "trap_sage.h"

double ts_compute_rms(const double *samples, size_t count);
double ts_compute_crest_factor(const double *samples, size_t count);
double ts_compute_burstiness(const double *samples, size_t count);
double ts_compute_active_fraction(const double *samples, size_t count, double threshold);
double ts_estimate_cycle_rate_cpm(const double *samples, size_t count, double sample_period_s);

#endif
