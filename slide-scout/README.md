# Slide Scout

USB-powered transmission densitometer for black-and-white and color negative film. Intended for darkroom printers who need repeatable, logged spot readings without a dedicated bench densitometer. Place a film area over the aperture; a baffled, constant-current white LED illuminates it and a TSL2591 measures transmitted light. The firmware reports relative optical density against an open-gate reference.

This is a low-cost reference design, not a calibrated instrument. It is not a medical, safety, or archival measurement device. No fabricated prototype or measured accuracy is claimed.

## Design targets

- Measurement range: optical density 0–3.0 (validated only after user calibration; sensor dark floor limits the upper range).
- Repeatability target: ±0.05 OD under fixed geometry and temperature; target only, not verified.
- Interface: USB CDC serial, 115200 baud; 3.3 V I2C.
- Power: USB 5 V, under 100 mA nominal.
- Use a neutral-density reference or step wedge to calibrate; this device does not provide traceable absolute density.

## Block diagram

```text
USB 5 V -> Pico USB/regulator -> 3V3 -> TSL2591 (I2C0)
                                +-> LED current resistor -> white LED -> diffuser/aperture
Pico USB CDC <- measurement CSV and calibration commands
```

## Parts / BOM summary

See `hardware/BOM.csv`. The Raspberry Pi Pico is a purchasable RP2040 module including flash, crystal, USB connector, and 3.3 V regulator; use its official board pinout and do not reproduce the bare-chip support circuitry. Sensor: TSL2591FN in a breakout/module with 3.3 V I2C compatibility. The listed breakout is a design-level selection; confirm module schematic and voltage before assembly.

## Pin assignment

| Signal | Pico pin | GPIO | Connection |
|---|---:|---:|---|
| I2C0 SDA | 6 | GP4 | TSL2591 SDA |
| I2C0 SCL | 7 | GP5 | TSL2591 SCL |
| LED drive | 16 | GP12 | LED + 220 ohm series resistor to GND |
| Ground | 3, 8 | — | Sensor ground |
| 3V3 OUT | 36 | — | Sensor supply (module must accept 3.3 V) |

Pico USB programming/debug uses the onboard USB interface; no GPIO is reserved for debug. GP4/GP5 are not boot-critical pins. Sensor module must include I2C pullups to 3.3 V; do not use a 5 V pullup module.

## Schematic and power

`schematic/connection-list.txt` is the electrical connection reference, not a KiCad schematic. Pico USB provides 5 V; the Pico module generates 3.3 V. LED current is approximately (3.3 V − 2.8 V)/220 Ω = 2.3 mA for a typical white LED; actual forward voltage varies. Sensor current and Pico current are within USB's 100 mA target. No battery or charging circuit is used. Keep the sensor baffled from ambient light; the LED and sensor face each other across a film gate.

## Firmware

`firmware/` is a Pico SDK C firmware project. It initializes I2C0, polls the TSL2591, and emits readings via USB CDC. The current firmware scaffold explicitly reports a sensor-read error rather than inventing a measurement; device-specific TSL2591 register integration remains a TODO and the project is not yet target-build verified. The density calculation helper has a host testable pure-C interface.

Build with Raspberry Pi Pico SDK 2.x installed and `PICO_SDK_PATH` set:

```sh
cmake -S firmware -B firmware/build -DPICO_BOARD=pico
cmake --build firmware/build
```

## Calibration and use

1. Assemble the optical head with a matte-black enclosure and fixed aperture.
2. Warm the LED for 60 seconds; ensure the film gate is empty and covered from ambient light.
3. Record the open-gate reference (I0), then measure the film patch (I).
4. Optical density is `log10(I0/I)`; use a step wedge to fit any instrument-specific correction curve.
5. Save emitted CSV with film stock, exposure, and processing notes.

## Validation status

Reference design; hardware unverified. No KiCad files are represented as valid: the connection list is an explicit netlist-style wiring reference. Firmware target compilation and sensor-register integration are unverified. The pure-C density helper and repository consistency checks are intended to be exercised on host. No measured precision or calibration traceability is claimed.

## Limitations

Sensor spectral response is not a standardized film densitometer response. Stray light, LED spectrum, geometry, negative gloss, and sensor saturation affect results. Use relative readings for personal printing workflow only. The TSL2591 intensity ratio requires dark-offset subtraction for high densities; an implementation must reject zero/below-floor readings rather than report finite density.

## License

MIT, consistent with the repository. Invented and maintained by jayis1.
