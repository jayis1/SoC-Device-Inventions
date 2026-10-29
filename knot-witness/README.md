# Knot Witness — Fishing-Knot Pull-Test Logger

Knot Witness is a low-voltage bench fixture that measures the peak tensile load and load-versus-extension-independent time trace of a fishing-line knot pulled to slip or failure. It helps recreational anglers and tackle makers compare knot-tying methods using repeatable, logged tests instead of judging by feel. The incumbent is a hand scale or an improvised hanging-weight test; the advantage is a timestamped force trace with consistent zeroing and a replaceable, documented calibration factor, rather than a claimed laboratory tensile tester.

## Design status

Reference design only. Firmware core and host tests are provided; Pico SDK target build, KiCad capture/ERC, mechanical validation, calibration, and physical testing have not been performed. The circuit description is a connection/netlist specification, not a native KiCad schematic. Do not treat readings as certified line strength or use this fixture for climbing, life-safety, or other safety-critical rope.

## Architecture

```text
Fishing line -> fixed anchor + hand-pull crosshead -> 100 N S-beam load cell
                                                  -> HX711 (10 SPS)
                                                  -> RP2040 / Pico
                                                  -> USB CDC CSV -> host logger
```

A slowly hand-pulled test intentionally trades speed for inexpensive, accessible hardware. The HX711's 10-sample/s mode is for comparative knot tests, not impact or high-speed fracture testing. A Pico board supplies the RP2040 and USB interface. The load cell and HX711 are powered from 3.3 V; bridge wiring must follow the purchased load-cell datasheet.

## SoC choice

Raspberry Pi Pico (RP2040) provides USB CDC for a local, offline CSV stream, deterministic GPIO bit-banging for HX711, and an inexpensive widely available development board. No radio, cloud service, battery, or network credentials are needed. The GPIO map below refers to Pico header GPIO numbers, not physical pin numbers.

## Pin assignments

| Pico GPIO | Signal | Connection |
|---|---|---|
| GP2 | HX711 DOUT | HX711 DOUT |
| GP3 | HX711 SCK | HX711 PD_SCK |
| GND | Ground | HX711 GND and bridge excitation return |
| 3V3(OUT) | 3.3 V | HX711 VCC; load-cell excitation only if cell datasheet permits |
| VSYS/USB | 5 V input | Pico USB connector; do not connect load cell to 5 V without verifying bridge ratings |

GP2/GP3 avoid the Pico's QSPI-reserved pins and BOOTSEL. The 4-wire cell connects E+, E−, A+, A− to HX711 E+, E−, A+, A− respectively. Confirm colors from the cell datasheet; wire colors are not standardized. HX711 RATE must be strapped low for 10 SPS.

## Power architecture and budget

USB 5 V powers the Pico board. Its onboard regulator provides 3.3 V to the RP2040 and HX711. Design budget: Pico board up to 100 mA while USB enumerated and firmware active (board-dependent); HX711 plus unloaded bridge budget up to 15 mA (cell-dependent); allow at least 150 mA from a compliant USB source. There is no battery charger or external regulator. Verify bridge excitation voltage/current against the selected load-cell datasheet and Pico regulator limits before assembly. This is a low-voltage instrument; it has no mains connection.

## BOM

See [hardware/BOM.csv](hardware/BOM.csv). Cost varies by supplier and load-cell capacity; the chosen 100 N S-beam cell and mechanical frame dominate cost. Estimated prototype total: US$25–45 at quantity one, excluding the frame and tools; pricing is an estimate, not a quote.

## Firmware

The board application uses the Raspberry Pi Pico SDK and outputs CSV over USB CDC. `firmware/core.c` contains host-buildable tare/calibration/peak logic; `firmware/main.c` acquires the HX711 and emits rows. On startup it tares from 32 samples, then emits `time_ms,raw,force_n,peak_n`. Calibration constant is a compile-time counts-per-newton value and must be established with known masses at the chosen fixture geometry. If HX711 data-ready does not arrive within 500 ms, the application reports a timeout and restarts acquisition; never substitute a fabricated force value.

Build (requires Raspberry Pi Pico SDK installed and `PICO_SDK_PATH` set):

```sh
cmake -S firmware -B firmware/build -DPICO_BOARD=pico
cmake --build firmware/build
picotool load -f firmware/build/knot_witness.uf2
```

The default calibration is intentionally zero/unconfigured. Set `COUNTS_PER_NEWTON` in `firmware/main.c` only after calibrating; until then force output is marked invalid. Host core test:

```sh
cc -std=c11 -Wall -Wextra -Werror firmware/core.c firmware/test_core.c -o /tmp/knot-witness-test
/tmp/knot-witness-test
```

## Assembly and calibration

1. Mount the S-beam cell between a rigid base and a guided hand-pulled crosshead, following its load-direction arrow and rated mounting hardware. Add guards against pinch points and a physical travel stop.
2. Attach the fixed line end to one cell fixture and the knot-side end to the moving fixture using repeatable, smooth-radius clamps; do not wrap line around sharp edges.
3. Wire cell to HX711 and HX711 to Pico per the pin table. Keep bridge leads short and twisted; secure strain relief so wire force does not load the terminals.
4. Connect Pico USB. With no load, start a fresh session; tare uses the first 32 stable readings.
5. Calibrate using several known masses / a force gauge through the actual fixture, checking force direction and linearity. Record calibration factor, cell serial, fixture geometry, and date externally. Never use a single point to claim traceable accuracy.
6. Test sacrificial line samples behind a transparent guard, pull smoothly, and keep hands/face out of the line path.

The test result is affected by line clamp slip, knot dressing, pull speed, temperature, fixture alignment, and HX711 sample rate. Compare multiple samples and report the distribution, not only the strongest result.

## API

USB CDC emits one CSV header followed by samples:

```text
time_ms,raw,force_n,peak_n
```

Force is reported only when calibration is configured. `raw` is the signed HX711 24-bit ADC count. `peak_n` is the highest valid force since startup. There is no command parser or network interface in this baseline.

## Limitations and safety

- 10 SPS sampling misses fast transients; this is not a universal tensile-testing machine.
- No physical prototype, calibrated accuracy, linearity, repeatability, fatigue, or temperature testing has been completed.
- A 100 N nominal cell is only an example; never exceed the exact cell, fixture, wire, or clamp ratings. Mechanical failure can eject parts or line.
- The design does not determine knot efficiency without matched unknotted-line control samples and repeat trials.
- Not a life-safety, climbing, fishing-regulation, or product-certification instrument.

## License

This device inherits the repository's license. Firmware files retain the project author attribution, jayis1.
