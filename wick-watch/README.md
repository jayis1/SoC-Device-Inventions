# Wick Watch

Offline capillary-rise comparator for testing how quickly narrow paper, textile, or porous-strip samples wick a dyed liquid. A Pico logs liquid-front arrival at five marked heights using a reflective IR sensor rail and exports timestamped CSV over USB CDC. It is a comparative bench instrument, not a standardized absorbency test.

Status: unbuilt reference design. Firmware core and host-side tests are provided; target build, electrical validation, calibration, and physical testing have not been performed. No PCB is supplied.

## Design

- SoC: Raspberry Pi Pico board (RP2040, 3.3 V GPIO), SDK 2.x.
- Sensor rail: five TCRT5000 reflective IR modules, each viewing a black/white contrast marker carried by a sample clip; digital outputs are active-low and use the modules' onboard comparator.
- User interface: BOOTSEL-independent GP14 start button to ground, GP15 status LED through 1 kΩ, USB CDC output.
- Power: USB 5 V powers Pico; sensor modules are powered from Pico 3V3 (verify module accepts 3.3 V). No battery or external voltage.
- Measurement: records the first stable low transition from each of five sensors. It reports elapsed milliseconds from start, plus timeout and sensor fault states. This does not infer liquid chemistry or absolute absorbency.

## Pin assignments

| Signal | Pico GPIO | Notes |
|---|---:|---|
| S1..S5 | GP2..GP6 | Active-low comparator outputs; 10 kΩ pull-ups to 3V3 |
| Start | GP14 | Momentary switch to GND; internal pull-up |
| Status LED | GP15 | 1 kΩ series resistor to GND |
| USB CDC | USB | Native Pico USB device |
| Sensor supply | 3V3(OUT) | Five modules; keep total below Pico board's 3V3 budget |
| Ground | GND | Common ground |

The Pico's GPIOs are not 5 V tolerant: do not power sensor modules at 5 V or connect their outputs to 5 V logic. Inspect the exact module before assembly; some boards' comparator pull-ups are tied to VCC.

## Build and use

Install Raspberry Pi Pico SDK 2.x and CMake/Ninja; set `PICO_SDK_PATH`, then:

```sh
cmake -S firmware -B build -DPICO_BOARD=pico
cmake --build build
```

Flash `build/wick_watch.uf2` by holding BOOTSEL while plugging in USB. Open the USB serial port at any baud rate (CDC ignores baud). Place a fresh strip vertically, position sensors at 10, 20, 30, 40, and 50 mm above the liquid surface, and connect the start button. Press start to begin; immerse the lower end to the marked datum. CSV rows contain elapsed_ms,sensor,event. No network, persistent storage, or credentials are used. Each run ends after all five events or a 120 s timeout.

## Calibration and limitations

Set comparator thresholds so dry background is high and the dyed wet front is low at every station. Verify each sensor with the actual strip and a water-based dye before logging. Keep optical geometry, strip width, liquid depth, and ambient conditions fixed. Results are comparative only and depend strongly on sample preparation and fluid. Do not connect to food, medical, or production quality systems without independent validation.

## Contents

- `schematic/netlist.md`: connection-level schematic specification (not a native KiCad drawing).
- `hardware/BOM.csv`: required parts and sourcing identifiers.
- `firmware/`: Pico SDK C application and build configuration.
- `docs/ASSEMBLY.md`, `docs/API.md`: assembly and CSV protocol.
- `scripts/analyze.py`: summarize event-time CSV locally.

## Validation record

Firmware is intended for Pico SDK 2.x; target compilation and hardware operation remain unverified. Host analysis script syntax and sample input are validated separately. Reference design only.
