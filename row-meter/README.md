# Row Meter

Offline planting-spacing wheel and field-note logger built around a Raspberry Pi Pico (RP2040). A small wheel rolls along a prepared garden row; a slotted optical encoder measures distance, while a button records planting events and the OLED reports the actual spacing. It helps gardeners and small-plot growers reproduce spacing without a phone or powered machinery.

Status: reference design; firmware core build-tested on host, not fabricated or calibrated. It is a measurement aid, not an agronomic recommendation. Accuracy depends on wheel diameter, slip, and calibration on the actual soil.

## Operation

1. Connect the Pico over USB (5 V input; board supplies 3.3 V logic).
2. Hold MARK for 2 seconds to zero the current row. Short press records a planting event.
3. Roll the wheel along the row. The display shows distance and spacing from the previous event; buzzer chirps at configurable target intervals.
4. Hold MARK and RESET during startup to enter calibration; rotate the wheel exactly one metre and press MARK.
5. USB CDC emits one CSV header and rows (`row,event,distance_mm,spacing_mm,tick_ms`). The device does not store data after power loss.

## Hardware and power

Use Raspberry Pi Pico (RP2040, board-level regulator and USB connector), a 3.3 V slotted optical interrupter, a 128×64 SSD1306 I²C OLED, one normally-open momentary MARK button, a passive piezo disc driven by a 2N3904 NPN stage, and a 10 kΩ pull-down. USB 5 V only; no battery or field wiring. Keep sensor leads short and add 100 nF across the sensor supply at the sensor.

Block diagram: 5 V USB → Pico board → 3V3 rail → optical encoder + OLED; encoder A/B → GPIO interrupts; MARK → GPIO pull-up; PWM GPIO → transistor → piezo; USB CDC → CSV host log.

## Pin assignments

| Signal | Pico GPIO | Connector/component | Notes |
|---|---:|---|---|
| I2C0 SDA | GP4 | OLED SDA | 3.3 V; OLED module pull-ups only |
| I2C0 SCL | GP5 | OLED SCL | 3.3 V; address 0x3C |
| Encoder A | GP10 | optical sensor A | Schmitt-conditioned 3.3 V output |
| Encoder B | GP11 | optical sensor B | quadrature direction |
| MARK | GP14 | switch to GND | internal pull-up; debounce 25 ms |
| PIEZO | GP15 | Q1 base through 4.7 kΩ | low-side NPN drive |
| 3V3 / GND | Pico pins 36 / 38 | sensor and display | common ground |

Do not connect 5 V logic to GPIO. USB and external power must not be applied simultaneously.

## Schematic and BOM

See `schematic/netlist.md` for the complete point-to-point connection list; it is the source design record (no KiCad capture supplied). BOM with quantities and part references is `hardware/BOM.csv`.

## Firmware

Portable C measurement core and host tests are in `firmware/`. `cmake -S firmware -B build && cmake --build build && ctest --test-dir build` builds the host logic. Target integration requires Pico SDK 2.x: add `main.c` to a Pico SDK project and link `pico_stdlib`, `hardware_i2c`, and `hardware_pwm`; target board build is not claimed here. The current source provides distance conversion, row/event tracking, and CSV formatting; sensor/display driver integration remains a target-side task.

## Calibration, limitations, safety

A 20-slot encoder on a 100 mm circumference wheel yields nominal 5 mm edge increments before quadrature interpolation; tire compression and soil slip dominate. Calibrate over a measured metre in representative soil. The implementation saturates counters rather than wrapping; invalid or missing sensor edges cannot be distinguished from a stationary wheel. Use only on low-voltage, dry garden equipment; clean soil from the wheel before storage.

## License

MIT. Invented and maintained by jayis1.
