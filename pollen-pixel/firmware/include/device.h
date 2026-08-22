#ifndef POLLEN_PIXEL_DEVICE_H
#define POLLEN_PIXEL_DEVICE_H

#include <stddef.h>
#include <stdint.h>

#define PP_FRAME_W 32
#define PP_FRAME_H 24
#define PP_MAX_PARTICLES 32
#define PP_MAX_HIST_BINS 8

typedef enum {
    PP_CLASS_UNKNOWN = 0,
    PP_CLASS_GRASS_POLLEN,
    PP_CLASS_TREE_POLLEN,
    PP_CLASS_WEED_POLLEN,
    PP_CLASS_FUNGAL_SPORE,
    PP_CLASS_DUST
} pp_particle_class_t;

typedef struct {
    uint16_t area_px;
    uint8_t min_x;
    uint8_t min_y;
    uint8_t max_x;
    uint8_t max_y;
    float circularity;
    float elongation;
    float uv_ratio;
    float edge_density;
    pp_particle_class_t klass;
} pp_particle_t;

typedef struct {
    uint8_t brightfield[PP_FRAME_H][PP_FRAME_W];
    uint8_t uv[PP_FRAME_H][PP_FRAME_W];
    float flow_lpm;
    float sample_seconds;
    float sampled_area_mm2;
    float inlet_efficiency;
} pp_capture_t;

typedef struct {
    pp_particle_t particles[PP_MAX_PARTICLES];
    size_t particle_count;
    uint16_t histogram[PP_MAX_HIST_BINS];
    float grains_per_m3;
    float dominant_um;
    float allergen_index;
} pp_report_t;

const char *pp_class_name(pp_particle_class_t klass);

#endif
