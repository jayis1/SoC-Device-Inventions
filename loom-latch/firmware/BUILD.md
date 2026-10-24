# Build requirements

Raspberry Pi Pico SDK 2.1.x (or compatible 2.x), CMake >= 3.13, Ninja or Make, and `arm-none-eabi-gcc` from the Pico SDK-supported toolchain. Set `PICO_SDK_PATH` to a locally installed SDK; this repository does not download dependencies.

Build: `cmake -S firmware -B build -DPICO_BOARD=pico` then `cmake --build build`. `pico_sdk_import.cmake` is expected in the firmware directory from the SDK checkout; copy it from the SDK `external/` directory before configuring. A host-only test does not validate the Pico target build.
