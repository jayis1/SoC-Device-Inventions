# Fold Ledger

Author: jayis1. Design reference; not fabricated or electrically validated.

Fold Ledger is an offline, USB-powered paper-folding setup logger for print shops, bookbinders, and community print studios. It records the order and elapsed time of manual folding passes from a guarded foot switch, with a second optical flag input for a simple paper-count pulse source. A technician can export timestamped events over USB CDC rather than tallying batches on scrap paper. It does not control a folder or certify finished work.

## Differentiator and user

The repository includes exposure and film workflow aids, but none records manual finishing-pass chronology as a local production trace. A small bindery currently uses a handwritten tick sheet; Fold Ledger makes a machine-readable, no-network batch record while leaving the operator and machinery in control. Unlike a generic tally counter, it records event type, sequence, elapsed milliseconds, and operator-marked batch boundaries.

## Block diagram

```text
USB 5 V -> Raspberry Pi Pico (RP2040) -> USB CDC JSONL
                                      <- guarded foot switch (GP14)
                                      <- isolated optointerrupter output (GP15)
                                      -> local status LED (GP16)
```

## Hardware and pin assignments

Target: Raspberry Pi Pico (RP2040 module, external QSPI flash on module). Power is USB 5 V through the Pico's onboard regulator; all inputs are 3.3 V logic. No battery, radio, mains connection, motor drive, or external power output. Use only a dry-contact switch on GP14 and an open-collector optocoupler output on GP15, pulled up to 3V3 with 10 kΩ. The optocoupler's LED/input side must be isolated from the machine sensor supply; do not connect machine mains wiring.

| Pico signal | Connection | Notes |
|---|---|---|
| GP14 | Foot switch to GND | Internal pull-up; debounce in firmware |
| GP15 | Optocoupler collector | External 10 kΩ pull-up to 3V3; emitter GND |
| GP16 | Status LED via 1 kΩ to GND | Active high |
| GP0/GP1 | UART0 test pads | Optional debug; not required for USB CDC |
| VSYS, GND | USB-derived supply and return | Do not inject another supply while USB is attached |

### Power budget

Pico is USB-powered, not battery-operated. Design estimate: RP2040/Pico 20–40 mA during USB CDC logging; LED adds up to 2 mA; switch and optocoupler logic together under 1 mA. Budget 50 mA at 5 V (0.25 W) for this logger, excluding the host and external isolated sensor. These are planning estimates, not measured values.

## BOM

| Qty | Part | Example orderable identity | Notes |
|---:|---|---|---|
| 1 | Raspberry Pi Pico | Raspberry Pi Pico, RP2040 module | Includes QSPI flash and 3V3 regulator |
| 1 | Momentary foot switch, normally open | Generic panel/foot switch | Dry contact only |
| 1 | PC817 optocoupler | PC817XxNSZ0F (verify package suffix) | Isolated pulse input; input resistor sized for external sensor voltage |
| 1 | 10 kΩ resistor | 0603, 1% | GP15 pull-up |
| 1 | 1 kΩ resistor | 0603, 1% | LED current limit |
| 1 | Green LED | 0603 | Status |
| 1 | USB cable | USB-A to micro-USB | Data capable |

Target incremental parts cost excluding Pico and enclosure: roughly US$4–10; estimate only, no live supplier quote.

## Firmware

Portable C core with Pico SDK 2.1.1, USB CDC stdio, GPIO edge polling, 5 ms debounce, and JSON Lines output. The event format is `{"ms":1234,"type":"foot"}` or `{"ms":1234,"type":"optical"}`; `{"ms":1234,"type":"batch_start"}` is emitted when the user holds the foot switch for two seconds. Timestamps are milliseconds since boot and wrap only after about 49 days; start a new batch after reboot. No storage is claimed: capture the CDC stream on the host.

Build (Pico SDK installed and `PICO_SDK_PATH` set):

```sh
cmake -S firmware -B firmware/build -DPICO_BOARD=pico
cmake --build firmware/build
```

Flash `firmware/build/fold_ledger.uf2` using BOOTSEL. Build status: Pico SDK is not installed in the authoring environment; target build not verified. Host core tests and Python script syntax are verified separately. Hardware and electrical behavior are unverified.

## Assembly and first use

1. Mount the Pico in an insulated enclosure with USB accessible; keep the foot switch cable separate from any machine wiring.
2. Wire the switch from GP14 to GND. Add the GP15 10 kΩ pull-up and connect only the optocoupler's transistor output to GP15/GND.
3. Before plugging in USB, check for shorts from 3V3 to GND and confirm the optocoupler input side has no conductive connection to the output side.
4. Flash firmware, open the USB CDC serial port at 115200 (baud is ignored by USB CDC), and press the switch. Capture one JSON object per line.
5. To mark a batch boundary, hold the foot switch for two seconds; ensure the operator's host capture is running first.

## Limitations

This is a prototype logger, not a production-counting or safety instrument. Optical pulses are debounced and may miss events faster than 5 ms apart. It has no nonvolatile log, clock-of-day, authentication, or calibration. Use only a SELV isolated optocoupler signal; never connect directly to powered industrial controls. No physical build, KiCad ERC, or calibration has been performed.

## Verification

The firmware event-state logic has a host test; the C source passes strict host syntax compilation. The schematic folder contains a reviewed connection specification, not native KiCad capture. No target build or physical test is claimed.
