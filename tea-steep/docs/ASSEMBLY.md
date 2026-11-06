# Assembly and bring-up

1. Use the Pico board's USB connector as the only supply. Confirm the TCS34725 breakout and every pull-up are 3.3 V-safe before attaching GP4/GP5.
2. Wire the parts according to `../schematic/netlist.md`; verify probe pinout from the actual probe documentation. Keep the optical path enclosed and mechanically fixed.
3. Install Pico SDK 2.x with its supported ARM GCC toolchain. Set `PICO_SDK_PATH`, then configure and build as shown in the top-level README. The CMake target requires that SDK; no SDK is vendored.
4. Flash the generated UF2 through BOOTSEL. Open USB CDC. The first line is a CSV header; sensor I2C initialization/read failures produce status `ERR`.
5. Current firmware does not yet implement DS18B20 1-Wire conversion/readout; temperature is deliberately marked invalid, so each record status remains `ERR`. Do not treat these records as validated measurement output. Implement and test that driver before relying on temperature.
6. Test with the LED off and on, then blank water, and repeatable tea samples. This repository run did not test a physical board, optical calibration, or leak protection.

Do not immerse the electronics or claim food-contact suitability from the waterproof probe coating alone.
