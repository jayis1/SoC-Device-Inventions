#pragma once
#include <stdint.h>
#define PATCH_CARD_MM_PER_COUNT (0.1000f) /* example only; calibrate actual wheel */
#define PATCH_CARD_I2C_ADDR (0x36u)
#define PATCH_CARD_GPIO_SDA (4u)
#define PATCH_CARD_GPIO_SCL (5u)
#define PATCH_CARD_GATE_A (14u)
#define PATCH_CARD_GATE_B (15u)
#define PATCH_CARD_LED_OK (16u)
#define PATCH_CARD_LED_ERR (17u)
#define PATCH_CARD_MIN_INTERVAL_MM (0.5f)
#define PATCH_CARD_MAX_INTERVAL_MM (25.0f)
#define PATCH_CARD_DEBOUNCE_MS (8u)
