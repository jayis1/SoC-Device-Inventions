#include "measurement.h"
#include <limits.h>
int bc_measure(const int32_t base[BC_DOT_COUNT], const int32_t top[BC_DOT_COUNT], struct bc_result *out) {
    if (!base || !top || !out) return -1;
    int64_t sum = 0;
    for (size_t i = 0; i < BC_DOT_COUNT; ++i) {
        int64_t h = (int64_t)top[i] - (int64_t)base[i];
        if (h < INT32_MIN || h > INT32_MAX) return -2;
        out->height_um[i] = (int32_t)h;
        if (i == 0 || h < out->min_um) out->min_um = (int32_t)h;
        if (i == 0 || h > out->max_um) out->max_um = (int32_t)h;
        sum += h;
    }
    out->mean_um = sum / BC_DOT_COUNT;
    return 0;
}
