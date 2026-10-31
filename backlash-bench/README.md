# Backlash Bench

Design status: reference design only; not fabricated, calibrated, or electrically validated. This low-voltage bench fixture helps hobbyists and repair technicians compare rotational lost motion in small gearboxes, couplings, and 3D-printer mechanisms. Instead of relying on subjective hand feel, two magnetic angle sensors measure input and output angles during a manual reversal sweep. Success targets (design goals, not measured results): resolve 0.1° encoder steps, capture at least 20 samples/s, and export a repeatable reversal trace over USB without a network connection. Off-the-shelf dial indicators can measure endpoint play, but paired angle traces preserve hysteresis and make comparisons easier.

The RP2040 is selected for its USB device support, two I²C controllers, and inexpensive module-based assembly. It is a microcontroller, not a single-chip SoC; the Raspberry Pi Pico module provides the MCU and onboard flash. This design is distinct from the repository's force/hinge fixtures: it measures angular input-output transmission deadband, not force or hinge resistance.

## Block diagram

```text
Input shaft + diametric magnet -> AS5600 --I2C0--+
                                                  +--> Raspberry Pi Pico RP2040 --> USB CDC CSV
Output shaft + diametric magnet -> AS5600 --I2C0--+
                                                  +--> local status LED
```

## Pin assignments

| Pico GPIO | Signal | Connection / constraint |
|---|---|---|
| GP4 | I2C0 SDA | Both AS5600 SDA; 3.3 V logic, shared bus |
| GP5 | I2C0 SCL | Both AS5600 SCL; 3.3 V logic, shared bus |
| GP6 | INPUT_DIR | Active-low direction marker switch to ground, internal pull-up |
| GP25 | STATUS_LED | Pico onboard LED, active high |
| USB | CDC console | Pico USB connector; no external UART required |

AS5600 devices both default to address 0x36, so the bus uses an I²C switch TCA9543A at 0x70. Channel 0 connects the input sensor and channel 1 the output sensor. Bus runs at 100 kHz. Use the breakout modules' 3.3 V-compatible I/O and disable/omit pull-ups that would pull SDA/SCL above 3.3 V.

## Power

USB 5 V powers the Pico; its regulated 3V3_OUT supplies the two sensor breakouts and TCA9543A. Design estimate: Pico active 25–40 mA; two AS5600 modules 5–10 mA total; switch and LED under 3 mA; total expected under 55 mA from 5 V after regulator losses. No battery, motor, or exposed hazardous voltage. Verify actual current and module pull-up circuitry before connection.

## BOM

See [hardware/BOM.csv](hardware/BOM.csv). Module vendors and exact breakout revisions vary; confirm 3.3 V compatibility and address-switch topology before assembly. Magnets and shaft couplers are mechanical consumables.

## Build and use

Install Raspberry Pi Pico SDK 2.1.1 and CMake, export `PICO_SDK_PATH=/path/to/pico-sdk`, then:

```sh
cmake -S firmware -B firmware/build
cmake --build firmware/build
```

Flash `backlash_bench.uf2` by holding BOOTSEL while connecting the Pico. Open the USB CDC serial port at 115200 baud. Align each diametric magnet over its sensor, secure the fixture, and slowly rotate the input through both directions while the output is mechanically coupled to the test transmission. Each output line is CSV: elapsed microseconds, input degrees, output degrees, direction. Do not force the fixture beyond the test mechanism's torque limit. Compare input/output angle at reversal; no absolute backlash accuracy is claimed without calibration against a reference rotary stage.

## Limitations and safety

Manual speed and coupling alignment affect results. AS5600 quantization, magnet eccentricity, shaft compliance, and fixture flex limit accuracy. Sensor modules may contain incompatible pull-ups; inspect before powering. Do not use for safety-critical inspection or claim calibrated metrology. No PCB layout is included; schematic/netlist is a wiring reference and requires KiCad capture and electrical review. No physical prototype or target compilation was performed in this run.

## License

MIT. Invented and maintained by jayis1.
