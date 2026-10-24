#include <stdbool.h>
#include <stdint.h>

typedef struct { bool armed; bool first_is_a; uint32_t started_us; } classifier_t;
/* Return 1 for a completed valid crossing, 0 otherwise. */
int classifier_edge(classifier_t *c, bool is_a, uint32_t now_us, uint32_t *duration_us, bool *a_to_b);
