#include <string.h>

#include "line_db.h"

float ls_nominal_impedance_for_type(const char *cable_type)
{
    if (strcmp(cable_type, "CAT5E") == 0 || strcmp(cable_type, "CAT6") == 0) {
        return 100.0f;
    }
    if (strcmp(cable_type, "ALARM_2C") == 0 || strcmp(cable_type, "ALARM_4C") == 0) {
        return 120.0f;
    }
    if (strcmp(cable_type, "IRRIGATION_2C") == 0) {
        return 75.0f;
    }
    if (strcmp(cable_type, "FENCE_WIRE") == 0) {
        return 300.0f;
    }
    return 100.0f;
}

float ls_expected_capacitance_nf_for_type(const char *cable_type)
{
    if (strcmp(cable_type, "CAT5E") == 0 || strcmp(cable_type, "CAT6") == 0) {
        return 4.5f;
    }
    if (strcmp(cable_type, "ALARM_2C") == 0) {
        return 2.2f;
    }
    if (strcmp(cable_type, "ALARM_4C") == 0) {
        return 3.5f;
    }
    if (strcmp(cable_type, "IRRIGATION_2C") == 0) {
        return 4.0f;
    }
    if (strcmp(cable_type, "FENCE_WIRE") == 0) {
        return 1.0f;
    }
    return 3.0f;
}
