# Patch Card

Offline quilting seam-allowance and stitch-length checker based on a Raspberry Pi Pico (RP2040). A fabric sample passes beneath a guarded, low-force measuring wheel; an AS5600 magnetic angle sensor reports travel, while two reflective optical gates observe stitch marks. It reports measured stitch pitch and seam-width variation over USB CDC. It is a comparative workshop gauge, not a certified textile inspection instrument.

Status: reference design; firmware core host-buildable, target build and physical accuracy unverified. No PCB layout is supplied. Use only low-voltage USB power; the device has no motor or fabric-feed actuator.

## Architecture

```text
Fabric path -> measuring wheel + diametric magnet -> AS5600 (I2C) --+
         two stitch-mark optical gates -> GPIO inputs ----------------+-> RP2040 Pico
                                                                       +-> USB CDC report
```

The operator manually moves fabric at a steady pace. Gate A/B are aligned across the seam; their alternating mark events provide stitch intervals while the encoder provides material travel. Measurements are raw comparative values; calibration against a ruler and known stitch sample is required.

## Parts and interfaces

| Ref | Part | Function |
|---|---|---|
| U1 | Raspberry Pi Pico, RP2040 | USB-powered MCU and USB CDC |
| U2 | AS5600L magnetic angle sensor breakout (3.3 V logic) | Measuring-wheel angle, I2C address 0x36 |
| S1/S2 | TCRT5000 reflective sensor modules configured for 3.3 V output | Detect contrasting stitch marks |
| D1/D2 | 3 mm LED | Local status |
| R1/R2 | 330 ohm | LED current limiting |
| C1 | 100 nF ceramic | Sensor rail bypass |
| M1 | 6 mm diametric magnet, on wheel hub | AS5600 target |

Use breakouts with documented 3.3 V-compatible outputs; do not connect 5 V logic to Pico GPIO. Keep magnet-to-sensor spacing within the AS5600 data-sheet range, then confirm AGC/magnitude via the sensor's diagnostic registers. Sensor module analog/comparator outputs are not interchangeable: use comparator output and set its threshold for the fabric mark.

## Pin assignments

| Pico GPIO | Connection |
|---|---|
| GP4 | I2C0 SDA, AS5600 SDA (4.7 kΩ pull-up to 3V3) |
| GP5 | I2C0 SCL, AS5600 SCL (4.7 kΩ pull-up to 3V3) |
| GP14 | Gate A digital output, active low |
| GP15 | Gate B digital output, active low |
| GP16 | Status LED through 330 ohm to ground |
| GP17 | Error LED through 330 ohm to ground |
| 3V3(OUT) | AS5600 breakout and optical modules only if explicitly rated for 3.3 V |
| GND | Common return |
| VSYS/USB | Pico power from USB; do not inject external supply concurrently |

The Pico's micro-USB connector provides power and USB CDC. GP14/15 use internal pull-ups; choose sensor outputs that safely sink to ground. Add 100 nF at each sensor breakout. Mount the wheel on a low-friction axle, with fabric contacting its rim without compressing or stretching the sample.

## Power and safety

USB 5 V is regulated on the Pico to 3.3 V. Estimated load is under 100 mA including two optical modules; this is an engineering estimate, not measured. No battery, mains, heater, motor, or exposed hazardous voltage is used. First power-up: inspect for shorts, connect the Pico alone to a current-limited USB source, then connect one sensor at a time and verify 3.3 V logic before attaching the sample.

## Firmware

The portable measurement core is C11 and host-buildable. Target integration uses Pico SDK 2.x and the `pico` board; `main.c` documents the GPIO/I2C adapter boundary. Build host core:

```sh
cmake -S firmware -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

A Pico SDK board application is not included yet; therefore the host core build does not validate target hardware or USB CDC integration. Use a known ruler length to set `mm_per_count` in `config.h`; never interpret uncalibrated output as absolute measurement.

## Calibration and use

1. Set sensor thresholds so each gate switches once per contrasting stitch mark; check with the sample stationary and moving.
2. Mark and measure a 100 mm fabric segment with a ruler, roll it through, and set wheel scale to match the encoder-reported distance.
3. Record the fabric type, tension, and needle/thread settings. Move slowly and consistently; discard intervals with missed/double gate transitions.
4. Compare mean pitch and range across samples; seam width is measured by aligning the two gates with the seam edges and is only as repeatable as the manual alignment.

## Limitations

Reflective contrast, fabric texture, wheel slip, skew, operator speed, and manual gate placement affect results. The design cannot identify stitch types or certify seam quality. No physical prototype or calibrated measurement has been tested.
