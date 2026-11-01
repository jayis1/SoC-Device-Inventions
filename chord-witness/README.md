# Chord Witness

USB-powered, offline guitar-string interval and tuning comparison fixture. It listens to a plucked string through a passive piezo contact pickup, estimates its fundamental with a bounded zero-crossing/period estimator, and reports frequency, nearest note, and cents deviation over USB CDC. Unlike the repository's acoustic room analyzer and musical instrument synthesizer, this is a single-string workshop calibration aid; it has no speaker, radio, or automated tuning actuator.

Target user: guitar repairers and instrument students who need repeatable before/after comparisons without a phone microphone in a noisy workshop. This is a comparative reference prototype, not a calibrated tuner or product safety instrument.

## Architecture

`piezo pickup -> 1 MΩ bias/input protection -> MCP6002 non-inverting buffer -> 3.3 V ADC -> RP2040 -> USB CDC text records`

Power: USB 5 V from a computer, Pico onboard 3.3 V regulator; never connect an external supply to the 3V3 pin. The piezo is passive and the circuit is low voltage. Input uses 100 kΩ series resistance, 3.3 V rail clamp diodes and 1 MΩ bias to mid-rail. Do not connect to an amplifier output.

### Pin assignments (Raspberry Pi Pico)

| Signal | Pico pin | GPIO | Notes |
|---|---:|---:|---|
| Buffered piezo | 31 | GP26/ADC0 | 0–3.3 V, biased near 1.65 V |
| UART debug TX | 1 | GP0 | optional 115200 8N1 |
| UART debug RX | 2 | GP1 | optional |
| Ground | 3/38 | GND | common analog return |
| USB CDC | micro-USB | — | firmware console |

ADC0 is sampled at 8 ksample/s in 256-sample windows. Expected measurement range is approximately 70–1000 Hz; reject low-amplitude or clipping windows. Firmware core is target-independent and syntax-checkable; Pico SDK adapter/build is not included in this reference prototype.

## BOM

See `hardware/BOM.csv`. Parts and pricing are indicative, not purchasing quotes. Use a Pico/RP2040 board with the exact pinout above. The passive pickup must be a high-impedance piezo disc/contact transducer.

## Build and use

1. Assemble the interface from `schematic/netlist.md`; check continuity and verify the ADC input remains between ground and 3.3 V before attaching the Pico.
2. Flash a Pico SDK application that samples ADC0 and calls `cw_estimate_frequency()` from `firmware/src/chord_witness.c`; the host-side estimator compiles with standard C11.
3. Connect USB and open the CDC serial port at 115200 baud. Pluck one string, wait for a stable record, and compare the frequency/cents fields before and after adjustment.
4. The Python helper generates a CSV example from a synthetic 110 Hz signal; it does not access hardware.

## Verification status and limitations

The portable C core builds with host CMake/GCC and the synthetic waveform helper runs and passes Python bytecode compilation. This is not a Pico SDK firmware image. Not fabricated, calibrated, or electrically validated. No KiCad executable is available; this folder contains an explicit netlist rather than claiming native KiCad/ERC validation. Frequency estimation is vulnerable to harmonics, pickup placement, weak plucks, and noise; use a known-frequency reference for calibration before quantitative work.

## Design review checklist

- [x] New use case compared with the README inventory and root device folders.
- [x] No radio, battery, mains, actuator, or medical function.
- [x] Documented GPIO/ADC mapping corresponds to Pico GP26/ADC0.
- [ ] Native KiCad schematic/ERC and physical hardware tests remain outstanding.
- [ ] Calibration against a traceable frequency reference remains outstanding.

Invented and maintained by jayis1.
