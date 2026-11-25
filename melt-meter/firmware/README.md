# Firmware core

This portable core accepts one decoded sensor sample per call. A Pico SDK adapter must configure SPI0 mode 0, 8-bit transfer chunks totaling 32 bits, and implement CDC output. Target adapter/build is not included; install Raspberry Pi Pico SDK 2.x and use the vendor's `pico_sdk_init` project template to provide hardware bindings. Do not claim target compilation from the host-only core check.
