#include "matrix.h"
void matrix_classify(const uint8_t samples[8][8], int8_t out[8][8]) {
    for (size_t d = 0; d < 8; ++d) {
        for (size_t s = 0; s < 8; ++s) {
            unsigned hits = 0;
            for (size_t k = 0; k < 8; ++k) hits += samples[d][k] != 0;
            out[d][s] = hits > 1 ? -1 : (samples[d][s] ? 1 : 0);
        }
    }
}