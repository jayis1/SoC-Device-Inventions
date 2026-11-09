# Firmware

RP2040 application for Raspberry Pi Pico SDK 2.x. Build: `cmake -S . -B build -DPICO_BOARD=pico && cmake --build build`. Configure `PICO_SDK_PATH` to a locally installed SDK. No dependency download is performed by this project. Host core test: `cc -std=c11 -Wall -Wextra -Werror -I src src/logger.c tests/test_logger.c -o /tmp/bw-test && /tmp/bw-test`.
