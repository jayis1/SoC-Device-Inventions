#include "thermals.h"

double ts_compute_delta_t_c(double upstream_temp_c, double downstream_temp_c)
{
    return upstream_temp_c - downstream_temp_c;
}

double ts_compute_upstream_superheat_c(double upstream_temp_c, double ambient_temp_c)
{
    return upstream_temp_c - ambient_temp_c;
}
