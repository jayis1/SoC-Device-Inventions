#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static bool active;
static uint32_t started;
static int last_level = -1;

void rain_scribe_start(uint32_t ms) {
    active = true;
    started = ms;
    last_level = -1;
    printf("{\"event\":\"BEGIN\",\"ms\":%lu}\n", (unsigned long)ms);
}

void rain_scribe_level(uint32_t ms, unsigned level) {
    if (!active || level > 4 || (int)level == last_level) return;
    last_level = (int)level;
    printf("{\"event\":\"LEVEL\",\"ms\":%lu,\"elapsed_ms\":%lu,\"level\":%u}\n",
           (unsigned long)ms, (unsigned long)(ms - started), level);
}

void rain_scribe_mark(uint32_t ms) {
    if (active) printf("{\"event\":\"MARK\",\"ms\":%lu,\"elapsed_ms\":%lu}\n",
                       (unsigned long)ms, (unsigned long)(ms - started));
}

void rain_scribe_end(uint32_t ms) {
    if (!active) return;
    printf("{\"event\":\"END\",\"ms\":%lu,\"elapsed_ms\":%lu}\n",
           (unsigned long)ms, (unsigned long)(ms - started));
    active = false;
}
