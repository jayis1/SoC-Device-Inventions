#ifndef PSYCHROMETRICS_H
#define PSYCHROMETRICS_H

#include "draft_ranger.h"

double dr_compute_dew_point_c(double temp_c, double rh_pct);
double dr_compute_condensation_margin_c(const dr_capture_t *capture, double dew_point_c);
double dr_compute_velocity_mps(double pressure_pa, double bridge_v);

#endif
