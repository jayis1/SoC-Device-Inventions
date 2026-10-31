# Build and connect

Provide a Pico module, sensor strip, five-channel comparator board, a diffused constant LED source, USB cable, and camera with a mechanical focal-plane shutter. Keep the gate strip outside the camera body, aligned at the film plane using a removable fixture; do not scratch the shutter curtains or force the camera open. The optical bench/fixture is not included in BOM.

Wire only 3V3, GND, GP2..GP6, and GP15 as specified in the connection spec. Verify each comparator output stays in 0–3.3 V before connecting the Pico. Power from USB only. Trim each threshold with the strip illuminated and then darkened to make each channel switch reliably; match thresholds across channels. No quantitative calibration is claimed.

Build with Pico SDK 2.1.1 and CMake 3.13+. Set PICO_SDK_PATH, run `cmake -S firmware -B build` and `cmake --build build`; drag `build/curtain_scope.uf2` onto the BOOTSEL drive. Those commands are specified but not executed here. Capture one test edge at a time first, then test all gates with a moving card before camera use.
