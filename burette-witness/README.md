# Burette Witness

Offline observer aid for manual acid–base titration. A stationary reflectance head records the color signal alongside manually entered volume marks; the operator confirms the endpoint and writes the burette reading. It does not dispense reagent, identify chemicals, or claim an automated endpoint. It is distinct from the repo's colorimeter (Tea Steep): it is a time/volume event recorder for a manually operated titration, not an infusion monitor.

## Design contract and limits

- Controller: Raspberry Pi Pico (RP2040), USB-powered 5 V, 3.3 V GPIO.
- Sensor: TCS34725 RGBC color sensor module (3.3 V I2C), viewing a fixed cuvette/titration vessel through an opaque clip-on hood. Sensor module address 0x29.
- User records burette volume and labels blank / sample / endpoint via USB CDC commands; firmware emits timestamped CSV with raw RGBC values. Endpoint determination stays with the human observer.
- No mains, battery, heater, pump, reagent contact, network, or medical/clinical use. Work with suitable PPE and chemical procedures; electronics remain outside the vessel and splash zone.
- Reference design only. Electrical compatibility of the chosen breakout, optical geometry, and calibration performance require bench validation; no fabricated hardware or chemical accuracy is claimed.

## Block diagram

`USB host -> Pico USB CDC -> event/CSV logger -> host file`

`Pico 3V3/GND + GP4/GP5 I2C -> TCS34725 breakout -> reflected vessel color`

## Pin and power plan

| Pico pin | Signal | Connection |
|---|---|---|
| 36 / 3V3(OUT) | 3V3 | sensor module VIN (must accept 3.3 V) |
| 38 / GND | ground | sensor GND |
| 6 / GP4 | I2C0 SDA | sensor SDA |
| 7 / GP5 | I2C0 SCL | sensor SCL |
| USB | power + CDC | host; current limited by host port |

Use an I2C breakout with pull-ups to 3.3 V, never 5 V. Do not connect a breakout powered from 5 V if its pull-ups also rise to 5 V. Add 100 nF at the sensor supply if absent on the module. Keep optical head wiring short. Pico has onboard USB power regulation; no external supply is required.

## Build and use

Firmware uses the Pico C/C++ SDK 2.x and CMake. Install the Raspberry Pi Pico SDK and toolchain, set `PICO_SDK_PATH`, then run `cmake -S firmware -B firmware/build -DPICO_BOARD=pico` and `cmake --build firmware/build`. Flash `burette_witness.uf2` by holding BOOTSEL while connecting USB. Open the USB CDC serial port at 115200 baud.

Commands: `sample,<label>` takes a color reading; `volume,<mL>` records the operator's current burette reading; `mark,<label>` records a blank/sample/endpoint/notes marker. Output is CSV. The supplied host-side firmware core tests cover event formatting and input validation; target compilation and physical sensor operation must be checked with the documented SDK and assembled hardware.

## Differentiation and feasibility

This small instrument does not replace visual judgment or a validated titrator: it preserves a synchronized, reproducible observation trail for teaching labs and comparative experiments. RP2040 USB CDC plus I2C is sufficient; no radio or high-voltage circuitry is needed. Main uncertainty is optical repeatability from vessel geometry, ambient light, and liquid turbidity, so the hood and blank controls are integral and absolute concentration claims are out of scope.

## Contents

- `firmware/`: RP2040 SDK application and host-testable C logging core
- `schematic/netlist.md`: connector-level wiring reference (not a KiCad schematic)
- `hardware/BOM.csv`: parts and sourcing notes
- `docs/ASSEMBLY.md`, `docs/API.md`: setup and serial protocol
- `scripts/check_design.py`: static pin/BOM/document cross-check

License: MIT. Author: jayis1. No PCB layout or fabrication files are included.
