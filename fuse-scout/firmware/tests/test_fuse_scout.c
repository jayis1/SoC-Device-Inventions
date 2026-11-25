#include <assert.h>
#include <stdint.h>
#include "../src/fuse_scout.c"

int main(void)
{
    const uint16_t samples[] = {100U, 200U, 300U};
    assert(fuse_scout_classify(0U) == FUSE_OPEN);
    assert(fuse_scout_classify(247U) == FUSE_OPEN);
    assert(fuse_scout_classify(248U) == FUSE_CHECK);
    assert(fuse_scout_classify(1240U) == FUSE_CHECK);
    assert(fuse_scout_classify(1241U) == FUSE_PASS);
    assert(fuse_scout_classify(4095U) == FUSE_PASS);
    assert(fuse_scout_average(samples, 3U) == 200U);
    assert(fuse_scout_average(0, 3U) == 0U);
    assert(fuse_scout_average(samples, 0U) == 0U);
    return 0;
}
