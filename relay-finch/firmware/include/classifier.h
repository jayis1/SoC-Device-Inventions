#ifndef RELAY_FINCH_CLASSIFIER_H
#define RELAY_FINCH_CLASSIFIER_H

#include "relay_finch.h"

void rf_classify_event(const rf_preset_t *preset,
                       const rf_features_t *features,
                       rf_decision_t *decision);

#endif
