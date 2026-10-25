# Quiet Cue

Quiet Cue is a USB-powered, silent visual task timer for shared workshops, study rooms, and sensory-sensitive users. A large encoder sets a countdown; a segmented LED bar shows remaining time without an audible alarm or network connection. Unlike the repo's sensors, detectors, and process instruments, this is a direct, user-operated accessibility/attention aid: no measurements are collected and no data leaves the device.

## Why this is distinct

Compared with Bench Buddy (electrical continuity mapping), Quiet Cue has no test probes or measurement function. Compared with Melody Sprite (musical instrument), its interaction is a single tactile setting control and a silent visual countdown. Its primary purpose is private, non-audio task transitions rather than sensing or actuation of another system.

## Block Diagram

```text
USB 5 V -> Pico USB input/regulator -> RP2040
                                  ├─ rotary encoder (A/B + push)
                                  ├─ 10-segment LED bar (2× 74HC595)
                                  └─ USB CDC status/configuration
```

## Design and pin assignment

Reference controller: Raspberry Pi Pico (RP2040, 3.3 V GPIO). The design intentionally uses the assembled module rather than a bare RP2040; USB, flash, clock, and regulator are provided by the module. GPIO assignments avoid the Pico's QSPI pins; no ADC, wireless, or boot-strapping behavior is used.

| Pico signal | Function | BOM reference |
|---|---|---|
| GP2 | Encoder A, external 10 kΩ pull-up to 3V3 | J2, R1 |
| GP3 | Encoder B, external 10 kΩ pull-up to 3V3 | J2, R2 |
| GP4 | Encoder push switch, active low, 10 kΩ pull-up | J2, R3 |
| GP6 | 74HC595 SER | U1, J3 |
| GP7 | 74HC595 SRCLK | U1, J3 |
| GP8 | 74HC595 RCLK | U1, J3 |
| 3V3 | U1/U2 VCC and pull-ups | U1, U2, R1-R3 |
| GND | common return | J1-J3, U1-U2 |

10 bar LEDs connect to U1 QA–QH and U2 QA–QB, each through its own 1 kΩ series resistor (R4–R13). U1/U2 are 74HC595D, powered at 3.3 V; output loading is limited to about 2 mA per illuminated LED. Firmware multiplexes the lit count over the bar by updating the shift-register outputs. The chain is daisy-chained U1 QH' to U2 SER. No LED current is sourced directly by the Pico GPIO.

## Power Architecture

USB 5 V from J1 feeds the Pico's documented USB input; the module generates 3.3 V. This reference design is USB powered only; it has no battery or charger. Budget estimate: Pico board 25–50 mA typical/80 mA design allowance, ten LED outputs at 2 mA each = 20 mA maximum, logic ICs under 2 mA. Design allowance is 102 mA at 5 V input including conversion margin. USB source should provide at least 500 mA. Runtime is not applicable. Estimate only; measure the final assembly before selecting a USB source for continuous use.

## BOM summary

| Ref | Qty | Part / MPN | Package | Notes |
|---|---:|---|---|---|
| U0 | 1 | Raspberry Pi Pico / SC0915 | module | RP2040 reference module |
| U1-U2 | 2 | SN74HC595D | SOIC-16 | 3.3 V shift registers |
| D1-D10 | 10 | generic green LED, 0603 | 0603 | Use low-current type; verify brightness at 2 mA |
| R1-R3 | 3 | 10 kΩ resistor | 0603 | Encoder pull-ups |
| R4-R13 | 10 | 1 kΩ resistor | 0603 | LED current limit |
| C1-C2 | 2 | 100 nF ceramic capacitor | 0603 | Local U1/U2 decoupling |
| J1 | 1 | USB micro-B cable / Pico connector | module | USB power/data, provided on Pico |
| J2 | 1 | 3-position encoder + switch | through-hole | External encoder |
| J3 | 1 | 6-pin header | 2.54 mm | Daisy chain, power, ground |

Parts cost and current distributor stock not checked. Confirm footprints and pin numbering against the selected manufacturer's datasheets before PCB layout.

## Firmware build/flash

`firmware/` contains a portable C timing and display core, not a complete Pico SDK application. Build its host checks with `cc -std=c11 -Wall -Wextra -Werror -c firmware/src/quiet_cue.c -o /tmp/quiet_cue.o`. For a physical Pico, integrate the core with Pico SDK 2.x GPIO/USB CDC bindings, build using `cmake -S firmware -B build -DPICO_SDK_PATH=/path/to/pico-sdk && cmake --build build`, then flash the resulting UF2 by copying it to the Pico BOOTSEL drive. The SDK adapter and UF2 have not been supplied or tested; do not interpret this as a ready-to-flash application.

## Assembly

Mount the Pico and two shift registers on a carrier PCB or breadboard. Add one 100 nF capacitor directly across each shift register's supply pins. Fit each LED with an individual resistor. Wire the encoder common to ground and the three switch contacts to GP2/GP3/GP4. Keep USB 5 V off the 3.3 V logic rail. Verify polarity and 3.3 V at U1/U2 before installing the Pico.

## Bring-up/Test

The portable core clamps duration to 1–120 minutes and computes a 10-step remaining-time display. Host compile check is required; hardware timing, encoder debounce and optical visibility remain unverified. Test all encoder transitions, reset behavior, and LED current on the assembled board. On reset all outputs should initialize low; firmware should not activate outputs until initialization completes.

## Safety

Low-voltage, USB-powered reference design; not a safety timer and not intended to control machinery, medication, or life-critical tasks. LEDs are indicators only. No audio, radio, microphone, camera, or data collection. Disconnect USB before wiring. Do not connect the 5 V rail to GPIO.

## Verification status

Reference design, not physically validated. No KiCad project is included because KiCad CLI was unavailable for this run; `schematic/netlist.md` is the proposed connectivity reference, not a fabrication-ready schematic. Firmware core is host-compilable, but no Pico SDK target build was available. BOM availability and PCB footprints are not independently verified.

## License

MIT; authored and maintained by jayis1.
