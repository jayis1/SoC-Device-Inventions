#ifndef FOLD_STOPWATCH_H
#define FOLD_STOPWATCH_H
#include <stdbool.h>
#include <stdint.h>
#define FS_MAX_GAP_US UINT64_C(60000000)
typedef struct { uint32_t sequence; uint8_t gate; uint64_t elapsed_us; } fs_event_t;
typedef struct { int last_gate; uint64_t last_time_us; uint32_t sequence; bool started; } fs_state_t;
void fs_init(fs_state_t *state);
bool fs_edge(fs_state_t *state, uint8_t gate, uint64_t time_us, fs_event_t *event);
#endif
