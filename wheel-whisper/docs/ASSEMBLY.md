# Wheel Whisper assembly and calibration guide

Designed by jayis1. This guide describes an unbuilt reference design; perform electrical, battery, RF, and mechanical review before fabrication.

## Assembly order

1. Inspect PCB fabrication against the conceptual netlist; verify the selected nRF52840/module reference design rather than copying generic QFN footprints.
2. Assemble passives, power path, and analog front end. Check 3V3 with a current-limited bench supply before fitting the SoC, OLED, or battery.
3. Fit the SoC/module and debug pads. Program a LED-only target adapter and confirm SWD recovery.
4. Fit OLED, Hall sensor, motor switch, and controls. Confirm no motor activation occurs during boot or SWD programming.
5. Fit the battery connector last. Verify charger termination, reverse-polarity protection, cell temperature, and shipping/off current.
6. Mount the piezo disc behind a compliant TPU/silicone tip. It should press lightly against one spoke without scratching coating or creating a sharp contact point.

## Calibration worksheet

The base firmware reports frequency, not absolute tension. Create a profile for each spoke family:

- Record manufacturer spoke type, free length, lacing, rim, hub, and side/group.
- On a known-good wheel, compare at least six positions against a calibrated commercial tension gauge.
- Fit only within the tested range and store the resulting table in the host app/profile script.
- Reject profiles with excessive residual error; use relative spread only.

## Functional checks

- Tap an isolated spoke 10 times; check repeatability after removing/reseating the clamp.
- Tap nearby crossed spokes and the rim; ensure quality gate rejects ambiguous captures.
- Sweep a rim magnet past the Hall sensor; confirm exactly one index event per pass.
- Verify BLE is opt-in, no capture is sent before pairing, and clear-route truly deletes local route data.
- Do not test on a wheel that will be ridden until a qualified builder has independently checked tension, true, dish, and spoke condition.
