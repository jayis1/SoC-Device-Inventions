#ifndef MATRIX_H
#define MATRIX_H
#include <stddef.h>
#include <stdint.h>
void matrix_classify(const uint8_t samples[8][8], int8_t out[8][8]);
#endif