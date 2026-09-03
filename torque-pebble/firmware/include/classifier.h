#ifndef TP_CLASSIFIER_H
#define TP_CLASSIFIER_H

#include "torque_pebble.h"

void tp_classify_event(const tp_event_t *event,
                       const tp_features_t *features,
                       tp_decision_t *decision);

#endif
