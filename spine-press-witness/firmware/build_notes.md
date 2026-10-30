# Build notes

Target: Raspberry Pi Pico / RP2040. Assumes Raspberry Pi Pico SDK 2.x installed and `PICO_SDK_PATH` points to that checkout; CMake and an ARM GNU toolchain must be available per the SDK setup guide.

    cmake -S . -B build -DPICO_BOARD=pico
    cmake --build build

Flash the resulting `spine_press_witness.uf2` by holding BOOTSEL while connecting USB and copying the UF2 to the mounted RPI-RP2 drive. The SDK import is intentionally not vendored. Do not use sdkconfig; this is Pico SDK, not ESP-IDF.
