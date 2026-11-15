# Assembly notes

1. Fit five normally-open reed switches along the outside of a transparent vertical standpipe at equal, measured intervals. Protect glass bodies against impact and strain-relieve leads.
2. Fit a buoyant float with an enclosed magnet. Move it through every level by hand and verify each switch closure with a meter. Adjust spacing/alignment before wiring the Pico.
3. Wire switch contacts to GP10..GP14 and ground; wire the momentary button between GP15 and ground. The Pico internal pull-ups establish idle-high inputs; never connect external voltage to these GPIOs.
4. Inspect for shorts and exposed conductors. Power the Pico from current-limited USB; check 3.3 V before connecting the sensor harness. Keep the board above splash level.
5. Record level spacing and standpipe inner diameter alongside exported data. Recheck the float for binding after filling and draining.

This is hand-wired reference hardware, not a fabricated PCB. Rain-water garden use only; do not use for potable-water installations or compliance measurements.
