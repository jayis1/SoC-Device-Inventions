# Crease Stop

USB-powered tabletop bookbinding registration aid. It measures the travel of a paper stack against a repeatable stop and records each press stroke, helping small-batch binders compare set-up repeatability without relying on visual estimates. Unlike repository force, crease, and press fixtures, Crease Stop measures the position of a sliding registration fence rather than force, paper transmission, or folding geometry.

## Design status

Reference design, not physically validated. Prototype is intended for a Raspberry Pi Pico (RP2040) on a solderless or hand-wired low-voltage fixture. No PCB or fabrication-ready KiCad project is supplied; `schematic/netlist.md` is the wiring specification. Firmware core is portable C and host-testable; Pico GPIO/USB integration is intentionally an adapter, not claimed complete. No calibration or accuracy claim is made.

## Architecture

A 10 kΩ linear potentiometer mechanically coupled to the fence produces a position signal. A 10 kΩ series resistor and 100 nF capacitor form a basic input filter; the potentiometer ends connect to 3V3 and GND. A normally-open momentary switch records a press when pulled low. Pico ADC converts position to a 12-bit raw count; USB CDC emits one CSV record per debounced press. The position is relative and requires fixture-specific calibration.

```text
USB 5 V -> Raspberry Pi Pico (onboard 3V3 regulator)
                         3V3 -> potentiometer -> ADC0/GP26
                         GP15 <- momentary switch to GND
                         USB CDC -> host CSV logger
```

## Pin assignments

| Pico signal | GPIO | Connection |
|---|---:|---|
| ADC0 | GP26 | Filtered potentiometer wiper |
| Input | GP15 | Pushbutton to GND, internal pull-up enabled |
| 3V3(OUT) | — | Potentiometer high end |
| GND | — | Pot low end and switch return |
| USB | — | Power and CDC data |

Do not apply more than 3.3 V to ADC0 or GPIO. The switch is a user input only; there are no actuators or mains connections. Keep the fixture mechanically isolated from the Pico and avoid pinch hazards at the sliding stop.

## Build and use

Install Raspberry Pi Pico SDK 2.x and CMake 3.13+, configure with `PICO_SDK_PATH` set, then build from `firmware/` using the documented command in `firmware/README.md`. The portable event core can be compiled on a host with `cc -std=c11 -Wall -Wextra -Werror -c src/crease_stop.c`. Wire the fixture, power only through USB, record the ADC endpoints at two known fence positions, and convert readings to millimetres on the host. Recheck endpoints after changing the mechanical linkage.

Output schema: `event,sequence,position_raw,button_state`; raw ADC values are 0–4095 and are not calibrated distances.

## BOM

See `hardware/BOM.csv`. Select a mechanically compatible 10 kΩ linear potentiometer and momentary switch; those mechanical choices depend on the fixture. The Pico is a module, not a bare RP2040 design.

## Limitations

No physical prototype, calibration, environmental characterization, or safety certification has been performed. This is a comparative registration aid, not a dimensional metrology instrument or a substitute for a calibrated gauge. Potentiometer wear, linkage backlash, ADC noise, and fixture compliance affect repeatability.
