# Coil Clock — offline magnetic pickup timing probe

Design status: concept, unbuilt. Author: jayis1.

A repairer winding small inductors or transformers needs to count turns without repeatedly stopping to inspect the bobbin. Coil Clock clips a low-voltage magnetic pickup beside a rotating winding jig, detects each magnet pass, and gives an audible tick plus USB-serial count. Unlike motion loggers in this repository, it counts magnetic index events; the target is a repeatable winding-jig revolution counter, not general motion or a sensor network.

## Architecture

```text
3.3 V USB -> Raspberry Pi Pico (RP2040) -> GPIO interrupt / debounce -> USB CDC
                                      -> GPIO15 -> 1 kΩ -> piezo
3-pin JST: 3V3, GND, GPIO14 <- SS49E linear Hall sensor (external pickup)
```

Use a small diametrically magnetized disk on the jig, with one north-to-south transition per revolution. SS49E output is analog, nominally centered near VCC/2; firmware samples it through GPIO26/ADC0 and detects a hysteretic crossing. This keeps sensor output within the Pico ADC range. No motor drive, mains, battery charger, radio, or cloud service is present.

## Pin assignments

| Pico signal | Connection | Notes |
|---|---|---|
| GPIO26 / ADC0 | SS49E OUT | 0–3.3 V analog; 10 kΩ series protection, 100 nF local filter |
| 3V3(OUT) | SS49E VCC | Sensor is 3.3 V powered |
| GND | sensor and piezo return | Common ground |
| GPIO15 | passive piezo through 1 kΩ | Local event tick; no external amplifier |
| USB | host | USB CDC logs; Pico USB programming |

Pico GPIO26–29 are ADC-capable; GPIO26 is ADC0. No boot or flash pins are repurposed. Sensor connector pin order viewed into the board: 1=3V3, 2=GND, 3=OUT; verify keyed housing orientation before assembly.

## Power and BOM

USB 5 V powers the Pico's onboard 3.3 V regulator. The Hall sensor and logic use the 3.3 V rail. Approximate budget: Pico 20–40 mA (USB active, workload dependent), SS49E <10 mA, piezo <3 mA intermittent; allow 60 mA USB headroom. No battery-life claim applies. Add 100 nF at sensor and 10 µF at connector rail. These are estimates, not measured values.

See `hardware/BOM.csv`. One-off cost estimate $8–15 using a Pico, sensor, magnet, connector, and passives; excludes enclosure and winding jig. At volume estimate $5–9, subject to board and distributor pricing. Nearest alternative is a commercial optical revolution counter; this open magnetic pickup avoids line-of-sight and is intended for small jig fixtures.

## Build and operate

Firmware uses Raspberry Pi Pico SDK 2.1.1. Install the SDK and toolchain per official Raspberry Pi documentation, then:

```sh
cmake -S firmware -B build -DPICO_SDK_PATH=/path/to/pico-sdk
cmake --build build
```

Flash `coil_clock.uf2` by holding BOOTSEL while connecting USB and copying the UF2 file. Open the Pico USB CDC serial port at 115200 baud (CDC ignores baud setting). Place exactly one magnet transition per revolution, power the sensor, and slowly rotate the jig. Check idle count remains unchanged; rotate five turns and expect five events. This is a bring-up procedure, not a completed hardware test.

## Failure behavior and limitations

Firmware requires the signal to cross both hysteresis thresholds and enforces a 20 ms lockout to reject chatter. Missing sensor, wrong magnet orientation, and wiring faults produce no counts; serial startup prints a diagnostic prompt. Noise can cause false counts; adjust magnet gap and threshold only after observing ADC values. Reset clears the count; persistence is not implemented. Counts represent detected index transitions, not guaranteed physical turns. Not a calibrated metrology instrument.

No PCB layout or enclosure is supplied. The proposed schematic is an explicit connection netlist, not a validated KiCad capture; no ERC/DRC or physical validation has been performed. Physical fit, sensor orientation, thresholds, and electromagnetic compatibility remain unverified.

## Verification

Host test: `gcc -std=c11 -Wall -Wextra -Werror -I firmware firmware/detector.c scripts/test_detector.c -o /tmp/test-coil-clock && /tmp/test-coil-clock`. A Pico-target SDK build and physical sensor test have not been run in this environment. Do not represent this reference design as a finished or calibrated product.
