#include <stdbool.h>
#include <stddef.h>

typedef struct { double rpm; double interval_cv_pct; size_t samples; } result_t;
/* intervals are milliseconds between revolutions; accepts 8..32 usable samples. */
bool analyze_intervals(const unsigned *intervals_ms, size_t count, result_t *out);
