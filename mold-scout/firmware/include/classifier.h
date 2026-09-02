#ifndef MOLD_SCOUT_CLASSIFIER_H
#define MOLD_SCOUT_CLASSIFIER_H

#include "mold_scout.h"

void ms_classify_window(const ms_sensor_window_t *window,
                        const ms_features_t *features,
                        ms_decision_t *decision);

#endif
