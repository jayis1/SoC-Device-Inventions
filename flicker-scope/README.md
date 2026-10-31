# Flicker Scope

A USB-powered bench probe for comparing visible-light modulation from lamps and displays. Clip the shielded photodiode head near a source; the device streams timestamped ADC samples over USB CDC so a host script can estimate modulation frequency and percent modulation. Unlike lux meters, it exposes the waveform, making PWM dimming and beat patterns inspectable without a scope.

Status: reference design, designed but not built. No optical safety or photometric accuracy claim. Do not use for occupational-safety certification.

## Architecture

```text
USB 5 V -> Raspberry Pi Pico (RP2040) -> USB CDC -> host analysis script
                    GPIO26/ADC0 <- photodiode + transimpedance amplifier
                    GPIO25 -> onboard LED (heartbeat)
```

Photodiode head: BPW34 reverse-biased from 3V3 through 100 kΩ; cathode to 3V3, anode to ADC node; 10 kΩ series between node and GPIO26 and 100 nF from GPIO26 to AGND. This simple resistor-load front end is intentionally limited to comparative, non-calibrated work; do not exceed 3.3 V at ADC0. Add a black tube to reject ambient light.

| Function | Pico signal | Board pin | Notes |
|---|---|---:|---|
| Photodiode ADC | GPIO26 / ADC0 | 31 | Input 0–3.3 V only |
| LED heartbeat | GPIO25 | 25 | Pico onboard LED |
| USB CDC | USB | — | Pico onboard interface |
| Analog ground | AGND | 33 | Sensor return |
| Sensor supply | 3V3 | 36 | Photodiode bias |

Power: USB VBUS powers the Pico; its onboard regulator supplies 3V3. Sensor load is nominally below 0.1 mA through 100 kΩ. No battery, charging circuit, or external mains connection. Power budget for the whole Pico is host-port dependent; no battery-life claim applies.

## Build and run

Requires Raspberry Pi Pico SDK 2.x, CMake, and an ARM GNU toolchain. From this directory: `cmake -S firmware -B firmware/build -DPICO_SDK_PATH=/path/to/pico-sdk`, `cmake --build firmware/build`; hold BOOTSEL while connecting the Pico and copy `firmware/build/flicker_scope.uf2` to RPI-RP2. Start `python3 scripts/analyze.py --port /dev/ttyACM0` (choose the actual port). Firmware emits `sample,<microseconds>,<adc12>` at nominal 500 samples/s, 256 samples per block. This is suitable for slow modulation demonstrations, not high-frequency flicker characterization.

## Verification status

The source and pin/netlist agreement are documented; this design has not been fabricated. Firmware build, KiCad ERC, optical calibration, and hardware behavior have not been verified. `schematic/NETLIST.md` is the authoritative connection description; no fabricated KiCad file is supplied. The Pico is a complete commercially available module rather than a custom RP2040 PCB.
