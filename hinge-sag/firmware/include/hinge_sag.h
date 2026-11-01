#ifndef HINGE_SAG_H
#define HINGE_SAG_H
#include <stddef.h>
#include <stdint.h>
/* Returns milli-degrees from gravity vector; 0 for invalid/near-zero vector. */
int32_t hs_angle_mdeg(float x_g, float y_g, float z_g);
typedef struct { int32_t first_mdeg, min_mdeg, max_mdeg; uint32_t count; } hs_capture_t;
void hs_capture_init(hs_capture_t *c);
/* Ignores samples after capacity; returns 1 when stored. */
int hs_capture_add(hs_capture_t *c, int32_t angle_mdeg, uint32_t capacity);
#endif
