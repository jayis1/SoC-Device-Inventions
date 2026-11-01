# Cord Gauge

Revision A — reference design, not physically validated.

A USB-powered benchtop fixture for comparing the stretch of small cord, elastic, fishing line, and sewing thread under repeatable low forces. A maker or repair shop can otherwise compare specimens by eye, which misses small but useful differences and leaves no record. This is a comparative instrument, not a certified tensile tester; it does not establish a material's rated strength.

## Why this SoC

The CH32V003F4P6 is an inexpensive 32-bit RISC-V MCU with a 12-bit ADC, timers, GPIO, and UART, sufficient for slow force/extension acquisition without radio or battery complexity. USB 5 V powers the fixture; serial output uses the MCU UART through a USB-UART adapter. This avoids claiming native USB capability.

## Architecture

```text
USB 5 V -> CH32V003 board (3.3 V) -> ADC + UART
                         |             |
  10 kg load cell -> HX711 bridge ADC  |-> USB-UART adapter -> host CSV
  sliding carriage -> 10 kΩ linear pot -> ADC
  tare/record buttons -----------------+
```

A fixed anchor holds the specimen; a hand-operated screw carriage stretches it. The load cell is in series with the fixed anchor and HX711; a linear potentiometer measures carriage displacement. Use only low forces; the 10 kg cell is a measurement range, not a safe load rating for the printed/mechanical fixture.

## Pin assignments

GPIO identifiers below are CH32V003 port pins (not package pin numbers). Confirm package pin numbering against the exact board before wiring.

| Signal | GPIO | Connection |
|---|---|---|
| HX711 DOUT | PC1 | 3.3 V logic input |
| HX711 SCK | PC2 | 3.3 V output |
| Displacement pot wiper | PA1 | ADC input; 10 kΩ ends to 3.3 V/GND |
| Tare button | PC3 | active low to GND, internal pull-up |
| Mark button | PC4 | active low to GND, internal pull-up |
| UART TX | PD5 | 3.3 V UART, 115200 8N1 via USB-UART RX |

The USB-UART adapter must be 3.3 V logic. Common ground is required. HX711 breakout supply and output logic must be verified; do not feed a 5 V DOUT signal to the MCU.

## Power and parts

Powered from a regulated USB 5 V source. Use a CH32V003F4P6 board with its own 3.3 V regulator; provide 100 nF local decoupling at the HX711 and load-cell excitation as specified by its breakout. No battery, charger, radio, or high-energy actuator is present. Estimated electronics load is below 100 mA; this is a design estimate, not a measurement. USB connector supplies logic only; mechanical loading is manual.

See [BOM](hardware/BOM.csv), [connection schedule](schematic/netlist.md), [assembly notes](docs/assembly.md), and [serial format](docs/api.md).

## Firmware and use

Firmware is portable C core source with a board-I/O adapter boundary. Build instructions and target status are in `firmware/README.md`. Calibrate the force channel using known masses and the displacement channel against a ruler before comparing specimens. Record specimen length, conditioning, preload, and calibration with each test. The fixture reports raw ADC-derived values; no material-grade or safety verdict is inferred.

## Verification status and limits

Firmware core host syntax check: intended for GCC; run `scripts/check.sh`. KiCad CLI/ERC and target toolchain were not available during authoring; `schematic/netlist.md` is an explicit connection schedule, not a KiCad schematic. No PCB, calibration, force accuracy, mechanical validation, or physical test is claimed. Keep fingers clear of the carriage and stop if the fixture bends or slips.
