#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Portable capture core; board I/O is intentionally supplied by the SDK adapter. */
typedef enum { HW_BEFORE = 0, HW_AFTER = 1 } hw_label_t;
typedef struct { hw_label_t label; uint16_t raw_angle; bool valid; } hw_capture_t;
typedef struct { hw_capture_t slots[2]; } hw_session_t;

void hw_session_init(hw_session_t *session) {
    if (session != 0) memset(session, 0, sizeof(*session));
}

bool hw_capture(hw_session_t *session, hw_label_t label,
                uint16_t raw_angle, bool sensor_ok) {
    if (session == 0 || !sensor_ok || raw_angle > 4095u ||
        (label != HW_BEFORE && label != HW_AFTER)) return false;
    session->slots[(unsigned)label].label = label;
    session->slots[(unsigned)label].raw_angle = raw_angle;
    session->slots[(unsigned)label].valid = true;
    return true;
}

bool hw_delta_counts(const hw_session_t *session, int16_t *delta) {
    if (session == 0 || delta == 0 || !session->slots[0].valid ||
        !session->slots[1].valid) return false;
    int32_t d = (int32_t)session->slots[1].raw_angle -
                (int32_t)session->slots[0].raw_angle;
    /* Shortest signed circular delta over 12-bit encoder range. */
    if (d > 2047) d -= 4096;
    if (d < -2048) d += 4096;
    *delta = (int16_t)d;
    return true;
}
