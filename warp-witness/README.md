# Warp Witness

Offline yarn linear-density and twist comparator for small spinning and weaving studios. The operator clamps a 100 mm yarn specimen, turns a calibrated capstan with a Pico-controlled stepper, and reads specimen length and turns while a 0.01 g load cell records mass. It reports tex (g/km), turns per metre, and repeatable sample records. Unlike Drop Index and Spine Press Witness, this measures textile mass-per-length and twist, not fluid flow or applied force.

Status: reference design; not fabricated or calibrated. Not a certified textile-testing instrument.

## First 10 minutes

Install Pico SDK 2.1.1 and CMake, connect the switches and load-cell interface, then build with `cmake -S firmware -B build -DPICO_SDK_PATH=/path/to/pico-sdk && cmake --build build`. Flash the UF2 to BOOTSEL. Intended USB CDC commands are `HELP` and `STATUS`; the CDC adapter and UF2 are not implemented, so this is not a ready-to-flash process. The present portable C core host-compiles as described below.

## Block diagram

```text
USB 5 V -> Raspberry Pi Pico (RP2040) -> USB CDC (planned)
                     ├─ GP2/3/4: encoder A/B/switch
                     ├─ GP6/7/8: 74HC595 serial/clock/latch -> 8 LED segments
                     ├─ GP10/11: HX711 DOUT/SCK <- 1 kg load cell
                     └─ GP14/15: DRV8833 IN1/IN2 -> geared capstan motor
```

## Pin assignment

Pico module pins (GPIO are 3.3 V only); no QSPI pins used. Separate regulated 5 V motor rail, common ground. Firmware outputs must remain low until initialization succeeds.

| Pico GPIO | Signal | Reference |
|---|---|---|
| GP2 | encoder A, 10 kΩ pull-up | J2/R1 |
| GP3 | encoder B, 10 kΩ pull-up | J2/R2 |
| GP4 | encoder switch, active low | J2/R3 |
| GP6 | 74HC595 SER | U1 |
| GP7 | 74HC595 SRCLK | U1 |
| GP8 | 74HC595 RCLK | U1 |
| GP10 | HX711 DOUT | U2 |
| GP11 | HX711 SCK | U2 |
| GP14 | DRV8833 IN1 | U3 |
| GP15 | DRV8833 IN2 | U3 |

I2C/SPI are unused. J3 is a 4-wire load-cell connection (E+, E−, A+, A−); check actual wire colors from the sensor datasheet. See `schematic/netlist.md` for proposed connectivity; it is not a native KiCad schematic.

## Power and operating limits

USB 5 V powers Pico and logic; separate regulated 5 V motor input (at least 1 A) feeds U3, with common ground. Approximate allowance: Pico up to 100 mA, LEDs 8×2=16 mA, HX711/logic <10 mA, motor up to 600 mA transient; estimated combined peak 0.73 A across separate rails. Use 100 nF local bypass and 47 µF at U3 VM. Verify motor stall current and driver thermal/current rating before assembly. Mechanical stops and a slip clutch are required. No battery.

The 100 mm specimen length is entered by the operator. Tex = mass_g / length_m; turns/m = turns / length_m. Measurement range is limited by selected load cell and fixture friction. Accuracy and throughput are unclaimed pending calibration.

## BOM (indicative; price/availability not checked)

| Ref | Qty | Part / MPN | Package | Description |
|---|---:|---|---|---|
| U0 | 1 | Raspberry Pi Pico SC0915 | module | RP2040 |
| U1 | 1 | SN74HC595D | SOIC-16 | LED shift register |
| U2 | 1 | HX711 | SOIC-16 | Load-cell ADC |
| U3 | 1 | DRV8833PWP | HTSSOP-16 | Dual H-bridge |
| J2 | 1 | Bourns PEC11R-4215F-S0024 | panel | Rotary encoder/switch |
| M1 | 1 | 5 V geared motor, ≤600 mA stall | assembly | Capstan drive; verify model/current |
| LS1 | 1 | 1 kg 4-wire load cell | bar | Mass sensing element |
| D1-D8 | 8 | low-current green LED, 0603 | 0603 | Status bar |
| R1-R11 | 11 | 10 kΩ/1 kΩ assortment | 0603 | Pull-up/current limiting |
| C1-C3 | 3 | 100 nF ceramic | 0603 | Local bypass |
| C4 | 1 | 47 µF, 10 V | radial | Motor bulk decoupling |
| J1/J3 | 2 | 2.54 mm headers | through-hole | Power/load cell |

See `hardware/BOM.csv` for cost estimates and manufacturer fields. Prices are indicative, not verified.

## Firmware

`firmware/src/warp_witness.c` is an SDK-independent C11 math core. Host check: `cc -std=c11 -Wall -Wextra -Werror -Iwarp-witness/firmware/include -c warp-witness/firmware/src/warp_witness.c -o /tmp/warp_witness.o`. The Pico GPIO, HX711, motor adapter, CDC protocol and target build are not included. Output units are milli-tex and milli-turns/metre; invalid input returns false.

## Assembly and safety

This is not a PCB release. Mount Pico and logic on a carrier, add local 100 nF decoupling, keep the motor on its own regulated rail and share ground. Confirm load-cell bridge wiring from its datasheet. Verify 3.3 V before connecting Pico GPIO. Fit a capstan guard, slip clutch and mechanical travel stop; keep loose clothing and fingers clear. Disconnect supplies before wiring. Not a safety-critical or certified instrument.

## Verification status

Reference design only. Portable computation core host-compiles. No native KiCad schematic, PCB layout, target SDK build, calibration, component stock check or physical test is provided. `schematic/netlist.md` is a connectivity proposal, not fabrication-ready documentation.

## License and author

MIT; authored by jayis1.
