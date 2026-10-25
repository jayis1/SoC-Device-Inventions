# Assembly

1. Build on perfboard with the Pico disconnected from USB. Check Q1 polarity: emitter to ground, collector to GP15; fit R1 from GP15 to 3V3.
2. Mount Q1 behind an opaque, non-marring hood that can sit over the camera's shutter opening. Avoid adhesive on delicate finishes.
3. Before connecting the sensor, inspect for shorts between 3V3 and ground. Power the Pico by USB and verify its 3V3 rail.
4. With USB disconnected, attach the sensor circuit; then reconnect. Confirm GPIO15 changes state when light is blocked/unblocked. If polarity differs, revise the logic only after confirming wiring.
5. Flash firmware and exercise it with a hand-operated light gate before testing a camera. Record multiple exposures at every dial setting; do not infer calibration from one shot.

Stop if the sensor wiring presents more than 3.3 V to a GPIO, the Pico heats, or the optical hood could contact/mar a shutter curtain. No battery charging, mains wiring, camera actuation, or hazardous voltage is present.
