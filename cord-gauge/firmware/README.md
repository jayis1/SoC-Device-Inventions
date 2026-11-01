# Firmware status

Portable acquisition core is provided in `main.c`. Target GPIO, ADC, HX711 bit-banging, and UART functions are platform adapter hooks; this package is not a complete vendor-SDK firmware image. Host syntax check is available via `../scripts/check.sh`. Target build is not verified because the CH32V SDK/toolchain is not installed in the authoring environment.
