#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "pipe_halo.h"

#ifdef __cplusplus
extern "C" {
#endif

void ph_classify_window(const ph_sensor_window_t *window,
                        const ph_features_t *features,
                        ph_decision_t *decision);

#ifdef __cplusplus
}
#endif

#endif
