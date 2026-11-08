# Firmware integration

Target: Raspberry Pi Pico, RP2040, Pico SDK 2.x (CMake-based).

This folder contains portable event logic only. Platform startup, ADC sampling, GPIO setup, debounce timing, and USB CDC transport remain Pico-SDK integration work and are not represented as implemented. Host-check the core with:

    cc -std=c11 -Wall -Wextra -Werror -c src/crease_stop.c -o crease_stop.o

No sdkconfig is used by Pico SDK.
