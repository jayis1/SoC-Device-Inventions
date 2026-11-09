# Hinge Cycle firmware core

Host test: `cc -std=c11 -Wall -Wextra -Werror -Isrc src/hinge_cycle.c tests/test_hinge_cycle.c -o /tmp/test_hinge_cycle && /tmp/test_hinge_cycle`.

Target adapter is intentionally not included. The core expects normalized 12-bit absolute-angle samples; a board integration must poll AS5600 register 0x0E/0x0F over I2C0 at GP4/GP5, handle I²C errors/timeouts, debouce the GP14 button, and emit CSV over Pico USB CDC. The reported angle uses sensor code directly, so mechanical zero calibration and wrap-aware unwrapping are required for accurate hinge travel. This core is a direction-reversal event demonstration, not validated product firmware.
