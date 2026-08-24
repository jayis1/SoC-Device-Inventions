#ifndef ACOUSTICS_H
#define ACOUSTICS_H

#include "draft_ranger.h"

typedef struct {
    double dominant_hz;
    double whistle_score;
} dr_acoustic_metrics_t;

void dr_analyze_acoustics(const dr_capture_t *capture, dr_acoustic_metrics_t *metrics);

#endif
