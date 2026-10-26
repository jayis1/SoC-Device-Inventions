# Escapement Ear

A USB-powered bench instrument that listens to a mechanical clock or watch through a contact piezo and reports beat interval, rate error, and beat asymmetry. It is a reference prototype: the electronics can timestamp impulses, but the mechanical coupling and measurement accuracy have not been validated on real movements.

## Who it helps / why an SoC

Clock hobbyists and repair learners can compare a movement before and after adjustment without buying a proprietary timing machine. Unlike a phone microphone, a contact pickup rejects room noise; the RP2040's hardware timer and interrupt capture make repeatable sub-millisecond event timing possible offline. Closest repo designs are acoustic analyzers and the mechanical load/rotation instruments; this one analyzes escapement event timing, not airborne sound, force, or motion.

## Design

Raspberry Pi Pico (RP2040 module) with a protected piezo input, passive RC envelope, MCP6001 non-inverting amplifier, adjustable comparator LMV331, and OLED header reserved for a future display revision. USB 5 V powers the Pico; the Pico 3V3 rail powers the low-current analog front end. No battery, radio, mains sensing, or actuator. Current firmware reports measurements over USB CDC; OLED is not yet driven.

```text
Movement case -> piezo -> 1M input + clamps -> MCP6001 gain ~11 -> LMV331 threshold -> GP14 edge IRQ
Pico GP4/GP5 I2C header -> optional SSD1306 OLED (not implemented in current firmware)
USB -> Pico VBUS -> onboard 3V3 regulator -> analog front end
```

### Pin map

| Pico GPIO | Function | Net | Notes |
|---|---|---|---|
| GP14 | rising comparator output | TICK | interrupt input, 3.3 V logic |
| GP4 | I2C0 SDA | OLED_SDA | 4.7k pull-up to 3V3 |
| GP5 | I2C0 SCL | OLED_SCL | 4.7k pull-up to 3V3 |
| GP25 | onboard LED | STATUS | active-high, optional |
| 3V3(OUT) | analog/OLED supply | +3V3 | total auxiliary load <20 mA target |
| GND | common return | GND | star-return piezo front-end |

GP14, GP4, GP5 and GP25 are ordinary RP2040 GPIO; none are QSPI pins or ADC pin assignments. Pico module exposes the RP2040's flash/crystal/regulator support; do not substitute a bare RP2040 without its reference circuitry.

## Measurement method

Timestamp threshold-crossing edges using a monotonic microsecond timer. Reject events closer than 80 ms, retain up to 64 intervals, and estimate the median interval. Rate in seconds/day is `(target_period_us / measured_period_us - 1) * 86400`; target period is configured in `firmware/config.h` and defaults to 500000 µs. The current firmware does not calculate beat asymmetry. These are screening estimates, not calibration-grade results. Contact location and threshold affect observed edge timing.

## Power

USB 5 V input through Pico USB connector, onboard regulator to 3V3; no cell or charging circuit. Design target <100 mA total, dominated by Pico and optional OLED; not measured. Use a current-limited USB source for first power. Analog input has series resistance and clamp diodes; never connect to a powered circuit or apply more than the piezo transient protection network is rated for. This is not a mains instrument.

## BOM

See [BOM](hardware/BOM.csv). The table contains exact component families; verify package/MPN substitutions before ordering. Module and display are breakout-board assembly, not a production PCB.

## Build and use

Requires Pico SDK 2.x and CMake. `cd firmware && cmake -S . -B build -DPICO_SDK_PATH=/path/to/pico-sdk && cmake --build build`. Flash `escapement_ear.uf2` to a Raspberry Pi Pico. Connect a passive piezo disc to the protected input, attach it gently to the movement case, set the movement's nominal beat interval in `config.h`, and observe at least 30 seconds of stable events. Do not use as a repair acceptance standard.

Firmware status: core timestamp/statistics module passes host GCC syntax check; target Pico SDK build not run. Schematic status: text netlist only; no KiCad validation or PCB layout. Hardware status: unbuilt and untested.

## Limitations and safety

Threshold detection is sensitive to mounting, movement amplitude, backlash, and noise. The prototype does not estimate lift angle or diagnose faults. No clinical or safety function. Use only passive piezo pickup; this design is not isolated for connection to powered equipment. All costs and current figures are unverified estimates.
