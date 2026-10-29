# Beat Window

Reference design: hardware and firmware unverified; not fabricated. This is a USB-powered optical beat-interval logger for repairers of mechanical clocks. It records the time between visible pendulum or balance-wheel passages so a clock can be compared before and after adjustment without a phone camera or network service. It reports measured intervals only; it does not infer accuracy or replace a calibrated time standard.

## Why it is distinct

The inventory already includes Escapement Ear, which detects mechanical escapement ticks acoustically. Beat Window instead uses a reflective optical gate to timestamp repeated visible passages and export raw intervals. Its measurement method and intended setup differ; it does not use a microphone or classify escapement sounds.

## Architecture and connections

A Raspberry Pi Pico (RP2040) samples a phototransistor divider on ADC0 and detects threshold crossings. USB CDC sends CSV to a host. A small LED illuminates the moving part; shield it from ambient light as needed.

```text
USB 5 V -> Pico onboard regulator -> 3.3 V
                           ├─ GP26/ADC0 <- phototransistor divider
                           ├─ GP15      -> indicator LED via resistor
                           └─ USB CDC   -> host CSV logger
```

| Signal | Pico pin | RP2040 GPIO | Connection |
|---|---:|---:|---|
| Optical sensor analog | 31 | GP26 / ADC0 | Phototransistor collector node; 3.3 V domain |
| Indicator LED | 20 | GP15 | GPIO through 1 kΩ resistor |
| USB serial | USB | — | Pico native USB |
| Sensor supply | 36 | 3V3(OUT) | Phototransistor divider only |
| Ground | 38 | GND | Common low-voltage ground |

The ADC input must remain between 0 and 3.3 V. This design is not isolated from the host. Do not connect it to mains-powered clock electronics or any external powered circuit.

## BOM and power

See `hardware/BOM.csv`. Pico is powered from a standard USB port; its onboard regulator supplies the sensor. The sensor divider is approximately 3.3 V / (10 kΩ + 10 kΩ) = 0.165 mA, and the LED is approximately (3.3 V − 2.0 V) / 1 kΩ = 1.3 mA when on (assuming a red LED with 2.0 V forward drop). Firmware blinks briefly; these are resistor-based estimates, not measured consumption. No battery or charging circuit.

## Firmware

`firmware/beat_window.c` is a portable C measurement core; hardware ADC sampling and USB CDC integration are deliberately left to a Pico SDK application. Build and run its host test:

```sh
cc -std=c11 -Wall -Wextra -Werror firmware/beat_window.c firmware/test_beat_window.c -o /tmp/beat-window-test
/tmp/beat-window-test
```

Input API takes monotonic microsecond timestamps at each detected crossing; output CSV is `event,interval_us`. Use a stable threshold/hysteresis in the target sampler to reject ambient flicker. Target integration, Pico SDK build, physical calibration and optical performance have not been verified.

## Schematic status and limitations

`schematic/connections.md` is a pin-level connection specification, not a KiCad schematic or PCB. The design provides no calibrated clock reference, uncertainty estimate, automated optical alignment, or electrical isolation. Verify sensor polarity and signal range before connecting a Pico; test with a known periodic light source before use on a clock.
