# Loom Mark

Status: design reference; firmware syntax-checked only, not built or physically validated.

## Problem and target user
A hand-weaver finishing a long warp needs to identify which crossing is the current pick after interruption, without counting rows manually or adding a powered display to the loom. Loom Mark records shuttle crossings as a local USB serial event stream.

## Why not buy an alternative
Commercial electronic row counters generally require manual button presses or are tied to knitting needles. This wired optical-gate design detects actual shuttle passage on a hand loom, works offline, and emits plain CSV; it is not a universal loom sensor and needs a suitable gate position.

## Success metric
At least 99 of 100 deliberate shuttle crossings are logged in a controlled bring-up test with gate alignment; target BOM under USD 20 excluding host computer. These are goals, not measured results.

## Concept and block diagram
A pair of modulated reflective optical gates detects shuttle passage through a loom-side guide. RP2040 timestamps ordered beam interruptions and reports events over USB CDC. No actuator, radio, or mains connection.

```text
USB 5 V -> Raspberry Pi Pico (RP2040) -> USB CDC host
                      GPIO 2/3 <- comparator outputs <- IR emitter/phototransistor gates A/B
                      GPIO 25 -> onboard status LED
```

## SoC choice and rationale
Raspberry Pi Pico (RP2040, QFN-56 on module) offers native USB device support and deterministic GPIO sampling, with a widely available SDK. The documented Pico module contains the required external QSPI flash; do not substitute a bare RP2040 without adding flash and clock circuitry.

## Pin table
| Signal | Pico GPIO | Direction | Notes |
|---|---:|---|---|
| Gate A comparator | 2 | input | 3.3 V logic, external 10 kΩ pull-up |
| Gate B comparator | 3 | input | 3.3 V logic, external 10 kΩ pull-up |
| Status LED | 25 | output | onboard LED active high |
| USB | fixed | CDC | USB connector on Pico |

## Power architecture
USB 5 V powers the Pico; its onboard regulator supplies 3.3 V. Gate comparators and sensor pull-ups use 3.3 V. Each IR LED runs from 3.3 V through 220 Ω (about 6 mA nominal per LED). Approximate total is 20–45 mA depending on LED duty and Pico activity; no battery or charger. Use a current-limited USB supply and do not connect external 5 V to GPIO.

## BOM summary
See `hardware/BOM.csv`; prices are rough estimates and availability varies. The optogates use common through-hole parts and a dual comparator. Exact comparator package and connector pinout are recorded there.

## Firmware architecture
`firmware/main.c` initializes GPIO inputs, detects ordered falling-edge transitions with a debounce interval, and emits CSV over USB CDC. Invalid or out-of-order edges reset the sequence. Event time wraps as an unsigned millisecond counter; the host should treat values as modulo 2^32.

## Build and flash
Install Raspberry Pi Pico SDK 2.x and its documented ARM GNU toolchain. Configure with `cmake -S firmware -B build -DPICO_SDK_PATH=/path/to/pico-sdk`, build with `cmake --build build`, then copy the resulting UF2 to a BOOTSEL-mounted Pico. The SDK was unavailable for this run; only a host syntax check with stub SDK headers was performed.

## Assembly and bring-up
Follow `docs/assembly.md`. First power from a current-limited USB port with gate LEDs disabled; confirm 3.3 V, then enable one gate and verify its output transitions. Align A then B along the shuttle path. Use `docs/api.md` to capture CSV. Stop if a GPIO exceeds 3.3 V or the module becomes hot.

## Testing
The pin/BOM consistency and host syntax checks are described in `docs/assembly.md`. Hardware detection rate, optical geometry, and firmware target build are unverified.

## Limitations
Requires clear optical access and a repeatable shuttle path; dark or highly reflective yarn, ambient sunlight, vibration, and double-passes can cause missed/duplicate events. It does not control or modify loom motion and does not infer fabric quality.

## Safety
Low-voltage USB-powered prototype only. IR light is low-power but do not stare into emitters at close range. Keep wiring and brackets clear of moving parts; no mains wiring. Not safety equipment.

## License
MIT; author: jayis1.
