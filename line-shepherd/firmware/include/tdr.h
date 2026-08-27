#ifndef LS_TDR_H
#define LS_TDR_H

#include "line_shepherd.h"

float ls_estimate_distance_m(const ls_capture_t *capture);
float ls_primary_reflection_v(const ls_capture_t *capture);
float ls_impedance_hint_ohm(const ls_capture_t *capture);

#endif
