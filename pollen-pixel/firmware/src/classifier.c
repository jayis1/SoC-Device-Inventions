#include "classifier.h"

pp_particle_class_t pp_classify_particle(pp_particle_t *particle)
{
    if (particle->area_px <= 10 && particle->uv_ratio < 0.35f) {
        return PP_CLASS_DUST;
    }
    if (particle->elongation > 1.7f && particle->uv_ratio > 0.60f) {
        return PP_CLASS_FUNGAL_SPORE;
    }
    if (particle->circularity > 0.72f && particle->uv_ratio < 0.45f) {
        return PP_CLASS_TREE_POLLEN;
    }
    if (particle->circularity > 0.60f && particle->uv_ratio >= 0.45f && particle->uv_ratio < 0.60f) {
        return PP_CLASS_GRASS_POLLEN;
    }
    if ((particle->edge_density > 0.015f || particle->area_px > 24) && particle->uv_ratio >= 0.60f) {
        return PP_CLASS_WEED_POLLEN;
    }
    return PP_CLASS_UNKNOWN;
}

float pp_allergen_weight(pp_particle_class_t klass)
{
    switch (klass) {
    case PP_CLASS_TREE_POLLEN: return 1.1f;
    case PP_CLASS_GRASS_POLLEN: return 1.4f;
    case PP_CLASS_WEED_POLLEN: return 1.8f;
    case PP_CLASS_FUNGAL_SPORE: return 0.8f;
    case PP_CLASS_DUST: return 0.2f;
    default: return 0.5f;
    }
}
