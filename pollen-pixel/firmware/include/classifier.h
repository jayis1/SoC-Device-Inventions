#ifndef POLLEN_PIXEL_CLASSIFIER_H
#define POLLEN_PIXEL_CLASSIFIER_H

#include "device.h"

pp_particle_class_t pp_classify_particle(pp_particle_t *particle);
float pp_allergen_weight(pp_particle_class_t klass);

#endif
