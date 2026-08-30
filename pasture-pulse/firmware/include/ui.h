#ifndef UI_H
#define UI_H

#include <stddef.h>

#include "pasture_pulse.h"

void pp_print_report(const pp_frame_t *frames, const pp_result_t *results, size_t count);

#endif
