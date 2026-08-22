#ifndef DSP_H
#define DSP_H

#include "pipe_halo.h"

#ifdef __cplusplus
extern "C" {
#endif

void ph_extract_features(const ph_sensor_window_t *window, ph_features_t *features);

#ifdef __cplusplus
}
#endif

#endif
