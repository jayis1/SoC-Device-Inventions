#include "../firmware/src/matrix.h"
#include <assert.h>
int main(void) {
    uint8_t sample[8][8] = {{0}}; int8_t out[8][8];
    sample[0][0] = 1; sample[1][1] = 1;
    matrix_classify(sample, out);
    assert(out[0][0] == 1 && out[1][1] == 1 && out[0][1] == 0);
    sample[0][1] = 1; matrix_classify(sample, out);
    assert(out[0][0] == -1 && out[0][1] == -1);
    return 0;
}