# Morse Mate

Design status: reference prototype; hardware is unverified and the firmware has not been built on this host.

## Who it is for

A learner or instructor who wants a quiet, offline Morse practice key that gives immediate tactile feedback without a phone, account, or network connection. Unlike phone apps and commercial radio keys, this USB-powered practice unit provides configurable local timing and a physical straight key in a repairable, open design. It is a training aid, not a radio transmitter or an assessment of operator proficiency.

## What it does

A RP2040 samples a normally-open straight key, measures press/release intervals, gives a short local piezo cue, and emits decoded characters as USB CDC text. It recognizes A–Z and 0–9 using a fixed 160 ms press-duration threshold and fixed 560 ms post-key gap; speed selection, punctuation, and word spacing are not implemented. A bounded input buffer prevents overflow. No RF transmitter, battery, wireless link, or stored personal data is included.

## Block diagram

```
5 V USB -> Raspberry Pi Pico (RP2040) -> USB CDC -> host terminal
                  | GPIO15 <- straight key to GND
                  | GPIO14 -> 1 kOhm -> passive piezo -> GND
                  + 3V3 -> key pull-up / Pico logic
```

## Pin assignments

| Pico GPIO | Function | Direction | Notes |
|---|---|---|---|
| GP15 | Key input | input | Internal pull-up; key shorts to GND; debounce in firmware |
| GP14 | Piezo cue | output | Series 1 kΩ; passive piezo only, no magnetic buzzer |
| USB | CDC console | bidirectional | Pico native USB; no external USB-UART bridge |
| SWD | Programming/debug | — | Pico SWD pads remain accessible |

Pico module pins are used; RP2040 QSPI flash pins are not exposed or reused. GP14/15 are ordinary GPIO and not boot-strapping pins.

## Power

USB 5 V powers the Pico board; the Pico's onboard regulator supplies 3.3 V. No charger or cell is fitted. Design estimates only: Pico board 20–40 mA during USB CDC operation, piezo under 1 mA through 1 kΩ, and USB active total under 50 mA expected. Idle USB power remains board-dependent; this is not a battery-life design. Use a current-limited USB host/charger. Never connect an external 5 V signal to a GPIO.

## BOM

See [BOM.csv](hardware/BOM.csv). The design uses a Raspberry Pi Pico module rather than a bare RP2040, so its flash, crystal, regulator, USB connector, and decoupling are already present. The passive piezo and straight key are low-voltage external parts.

## Firmware build and flash

Requires Raspberry Pi Pico SDK 2.1.1 and its documented CMake/ARM GNU toolchain setup. In `firmware/`:

```
cmake -S . -B build -DPICO_SDK_PATH=/path/to/pico-sdk
cmake --build build
```

Hold BOOTSEL while connecting USB, then copy `build/morse_mate.uf2` to the RPI-RP2 drive. Open a USB CDC terminal at 115200 baud; decoded characters are printed as lines. SDK/toolchain were unavailable here, so a firmware build and device smoke test were not performed.

## Timing and output

The current implementation classifies a key closure shorter than 160 ms as a dot and a longer closure as a dash. It finalizes a character after a fixed 560 ms key-up interval. It recognizes A–Z and 0–9; unsupported sequences yield `?`. This is a fixed-timing prototype, not a configurable-speed decoder.

## Schematic and construction

The authoritative pin-level connection list is [netlist.md](schematic/netlist.md). This run does not include a captured KiCad schematic; the schematic directory explicitly records this limitation rather than supplying a misleading placeholder. Assemble the three low-voltage connections exactly as listed. Mount the key so its contacts cannot short adjacent Pico pins; insulate exposed conductors and strain-relieve the USB cable. Confirm piezo polarity if using a polarized part.

## Limitations and safety

This is a reference design, not a validated product: no physical prototype, EMC, USB compliance, timing accuracy, or accessibility study has been performed. Key timing is an educational approximation and should be tuned to the learner. Do not connect this device to a radio PTT or transmitter. The passive piezo may be omitted; never substitute a high-current speaker without a driver stage. No medical, safety, or radio performance claims are made.

## License

MIT. Authored by jayis1.
