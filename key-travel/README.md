# Key Travel

A bench fixture for comparing mechanical keyboard switch force–travel curves without a motorized test stand. Turn a fine-pitch screw by hand; the instrument logs load-cell force against screw travel over USB as CSV. It is a measurement aid for hobbyists and switch repairers, not a calibration instrument.

## Problem and design decision

Switch feel is often described with a single actuation-force number, which hides preload, tactile peaks, hysteresis, and return behavior. Key Travel records the full loading and unloading curve. The deliberate tradeoff is a hand-driven micrometer screw: it avoids motor control and pinch hazards while keeping the mechanism inexpensive; the operator controls speed and the result is not a standardized force test.

## Specifications

| Item | Design target |
|---|---|
| SoC | Raspberry Pi Pico board (RP2040, 3.3 V logic) |
| Input | 1 kg bar load cell through HX711; AS5600 magnetic angle encoder on screw |
| Motion | Manual M6×0.5 lead screw; 12-bit angle measurement, nominal 0.5 mm/rev travel |
| Interface | USB CDC serial; newline-delimited CSV records |
| Supply | USB 5 V only; no battery, radio, or exposed mains |
| Status | Reference design; firmware/toolchain build and physical hardware are unverified |

## Block diagram

```text
USB host ⇄ RP2040 Pico ── I²C (GPIO4/5) ── AS5600 + diametric magnet on screw
                     └── GPIO10/11 ── HX711 ── 1 kg load cell ── switch platen
Manual M6×0.5 screw ─────────────────────────────── pushes switch vertically
```

## Pin assignments

| Pico GPIO | Connection | Notes |
|---|---|---|
| GP4 / GP5 | I²C0 SDA/SCL → AS5600 SDA/SCL | 3.3 V bus; 4.7 kΩ pull-ups to 3V3; address 0x36 |
| GP10 | HX711 DOUT | Digital input; sample-ready goes low |
| GP11 | HX711 PD_SCK | Keep clock pulses below 60 µs to avoid power-down |
| USB | Host serial and power | Use the Pico's onboard USB connector |
| 3V3 / GND | Encoder and HX711 logic | HX711 module analog/load-cell excitation supply must follow its module's documented range; do not apply 5 V to Pico GPIO |

The Pico's QSPI flash pins are not used. No boot-strapping GPIO is relied upon. Confirm the exact Pico board revision and module schematic before wiring; GPIO numbering means RP2040 GPIO, not physical header pin numbers.

## Power architecture and budget

USB 5 V powers the Pico board; the board's 3V3 rail powers AS5600 logic and HX711 logic. The load-cell bridge excitation is supplied by the HX711 board as specified by that board. There is no charger or second power source. Keep motor/actuator outputs absent.

Estimated 3V3 external load: AS5600 6 mA typical + HX711 1.5 mA typical + pullups ≤1 mA average = about 8.5 mA. Pico board current varies with USB activity; budget 40 mA typical system and reserve at least 100 mA from the host port. USB bus power only. These are planning estimates, not measured values. Power-cycle and check rail voltage/current before connecting a host used for critical work.

## BOM summary

See [`hardware/BOM.csv`](hardware/BOM.csv). Major devices are Raspberry Pi Pico, HX711 module, AS5600 breakout, 1 kg load cell, and two 4.7 kΩ I²C pull-ups. Generic mechanical hardware is deliberately described by specification rather than an unverified supplier SKU.

## Firmware and data

The Pico SDK program tares the force channel on command, reads the AS5600 raw angle and HX711 samples, and emits records in the form `sample_ms,travel_mm,force_N,direction`. Force requires a calibration factor obtained using known weights. Travel is computed from unwrapped screw angle and the measured lead; initialize at the top stop before each sweep. An `M` record flags invalid sensor reads instead of substituting zero. Reverse the screw to capture hysteresis.

This repository provides the pin-level wiring/netlist rather than a KiCad capture. No `.kicad_sch` is claimed. The schematic/netlist and mechanical fixture are a prototype design, not manufacturing documentation.

## Build and flash

Install Raspberry Pi Pico SDK 2.1.1 and its documented ARM GCC/CMake/Ninja toolchain. From `firmware/`, set `export PICO_SDK_PATH=/path/to/pico-sdk`, configure with `cmake -S . -B build -DPICO_BOARD=pico`; build with `cmake --build build`; put the Pico in BOOTSEL mode and copy `build/key_travel.uf2`. Toolchain build has not been run in this environment. Host-side host-tool syntax check is reported below; hardware operation is unverified.

## Assembly and first use

Follow [`docs/assembly.md`](docs/assembly.md). Secure the fixture to a stable bench, align the load cell axially, keep fingers clear of the screw/platen, and begin with a low-range known mass. Do not exceed the 1 kg cell rating. Tare without a switch installed; calibrate using a known mass; then collect a slow down/up sweep. See [`docs/api.md`](docs/api.md) for serial records.

## Limitations and verification status

- Designed: wiring, protocol, and calculations are specified. Firmware is written for Pico SDK but not compiled here; the Pico SDK and ARM cross-compiler are unavailable.
- Schematic status: netlist-level only; no KiCad ERC/DRC or PCB layout performed.
- No bench measurements, force accuracy, repeatability, or safety certification are claimed.
- Screw backlash, compliance, off-axis loading, operator speed, sensor noise, and cell creep affect results. Travel is screw displacement, not direct keycap displacement; subtract fixture compliance only after independent characterization.
- AS5600 angle wrap and magnet alignment require a zeroing operation. Calibration is user supplied and stored only for the current run.
- A single manual sweep is not comparable to standardized switch testing; use the same fixture, rate, mounting, and calibration for comparisons.

## License

MIT, consistent with the repository. Invented and maintained by [jayis1](https://github.com/jayis1).
