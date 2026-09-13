#ifndef LEAK_MODEL_H
#define LEAK_MODEL_H
#include <stddef.h>

typedef struct { float pressure_pa_s, ringdown_ms, pressure_z, acoustic_z, confidence_pct; int pass; } sw_result_t;
/* Inputs are equally spaced samples. pressure is gauge Pa and temperature is Celsius. */
sw_result_t sw_analyze(const float *pressure, const float *temp_c, size_t n, float dt_s,
                       float reference_leak, float reference_tau, float leak_sigma,
                       float tau_sigma, float z_limit);
#endif
