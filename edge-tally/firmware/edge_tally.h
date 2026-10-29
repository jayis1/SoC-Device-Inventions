#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool stable;
    bool candidate;
    uint32_t since_ms;
    uint32_t count;
} edge_tally_t;

void edge_tally_init(edge_tally_t *s, bool initial_high, uint32_t now_ms);
void edge_tally_sample(edge_tally_t *s, bool high, uint32_t now_ms);
