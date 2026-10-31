# Validation record

Status: firmware source drafted; target build and physical hardware unverified. No Pico SDK path was supplied, and no KiCad schematic/ERC exists. The repository release should describe this as a module-level reference design, not fabrication-ready hardware.

Run `python3 -m py_compile scripts/plot_log.py` and `python3 scripts/plot_log.py --help` from this directory. Firmware build command: `cmake -S firmware -B build -DPICO_SDK_PATH=/path/to/pico-sdk && cmake --build build`. Validate ADC framing against the MCP3008 datasheet and test SPI output on hardware before relying on readings.
