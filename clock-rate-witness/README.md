# Clock Rate Witness

Clock Rate Witness is a low-voltage optical timing logger for comparing the rate stability of mechanical and quartz clocks. A reflective sensor watches a removable matte marker on the seconds hand (or a once-per-minute dial marker); the RP2040 timestamps each crossing and exports raw intervals over USB CDC. It is a comparative timing aid, not a calibrated time standard.

## Differentiator
Unlike Escapement Ear (contact-piezo tick intervals) and Beat Window (pendulum passages), this observes actual displayed hand motion without touching the mechanism. It reports hand-rate error and interval variation, not acoustic beat quality or pendulum period.

## Block diagram

```text
USB 5 V -> Raspberry Pi Pico (RP2040, onboard 3V3)
                       GP26/ADC0 <- QRE1113GR reflective optosensor + analog output
                       GP15 <- RUN button (active low)
                       USB CDC -> timestamped CSV
```

## Wiring and pin assignments
Use a Raspberry Pi Pico SC0915 module, 3.3 V GPIO. The optical head is positioned 2–5 mm above the dial and aimed at a small matte-white marker on the seconds hand against a dark background. Set the sensor potentiometer so the analog output is below 0.5 V on dark and above 2.0 V on the marker; never exceed 3.3 V at GP26. Sample at 1 kHz and detect a rising threshold crossing with hysteresis. GP15 is pulled up internally and the RUN button connects it to GND. USB is the only power input. No clock mechanism is electrically connected.

| Signal | Connection |
|---|---|
| Pico GP26 / ADC0 | Sensor module analog output, 0–3.3 V only |
| Pico GP15 | RUN pushbutton to GND |
| Pico 3V3 | Sensor VCC |
| Pico GND | Sensor GND, common |
| USB | Pico power and CDC data |

## Power and BOM
USB-powered reference design. The Pico's onboard regulator powers the sensor; estimated load <100 mA at 5 V (measure assembled current). No battery, mains connection, actuator, or wireless radio.

| Ref | Qty | Part / MPN | Notes |
|---|---:|---|---|
| U1 | 1 | Raspberry Pi Pico SC0915 | RP2040 module, USB micro-B |
| U2 | 1 | QRE1113GR reflective sensor | Vishay, analog reflective sensor |
| R1 | 1 | 10 kΩ potentiometer | Sensor threshold/gain adjustment; module may integrate it |
| C1 | 1 | 100 nF ceramic | Across sensor VCC/GND |
| SW1 | 1 | momentary normally-open pushbutton | RUN input to ground |
| J1 | 1 | USB micro-B cable | Power/CDC |

The design assumes a QRE1113-based analog breakout with a 0–3.3 V output; breakout circuit and output range must be checked before connection. No PCB is supplied: use a breadboard or carrier board and keep the optical head mechanically stable.

## Firmware
`firmware/src/clock_rate.c` provides a portable interval/classification core. Build its host check with `cc -std=c11 -Wall -Wextra -Werror -c firmware/src/clock_rate.c -o /tmp/clock_rate.o`. Integrate the core with Pico SDK 2.x ADC, GPIO and USB CDC bindings; those target bindings are not included, so this repository does not claim a ready-to-flash image. Firmware should sample at 1 kHz, apply rising-edge hysteresis, timestamp with the RP2040 monotonic timer, and emit CSV fields `sequence,elapsed_us,interval_us,rate_error_ppm`.

For one marker per second, rate error ppm = (measured interval / 1,000,000 − 1) × 1,000,000. A once-per-minute marker requires 60,000,000 µs nominal interval. Discard the first edge after arming; report missing crossings as gaps, never interpolate them as observations.

## Assembly and use
1. Connect sensor VCC/GND and analog output to Pico 3V3/GND/GP26; verify sensor output stays within 0–3.3 V before connecting GP26.
2. Connect RUN button from GP15 to GND. Install the 100 nF capacitor at the sensor.
3. Fix the sensor on a non-marring stand; apply a small matte marker to the seconds hand only if safe and removable. Do not obstruct or load the hand.
4. Connect USB, open the virtual serial port, arm measurement, then collect at least 10 minutes. Compare CSV against a trusted reference clock; do not infer absolute accuracy from USB timing alone.

## Validation and limitations
Reference design only; hardware, calibration, and target firmware are unverified. No KiCad CLI or ARM embedded compiler was available in the authoring environment. The portable core is host syntax/build checked. Optical geometry, marker contrast, hand wobble, missed/double detections, and clock gear-train irregularity can bias results. Not suitable for watch movements or precision certification. If edges become implausibly short, firmware must reject them using a documented debounce threshold and retain a fault counter rather than silently correcting data.

## Safety
USB low voltage only. Do not open, modify, or electrically connect to the clock. Do not attach anything that adds appreciable load to a clock hand. Disconnect USB before rewiring. This is not a certified timekeeping instrument.

## License
MIT; authored and maintained by jayis1.
