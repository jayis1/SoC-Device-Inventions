# Crease Meter

A low-voltage comparative paper crease/score evaluation instrument for bookbinders and print shops. It illuminates an unprinted paper coupon and samples transmitted light while the operator bends a scored specimen over a repeatable radius. It reports relative opacity change; this is not an absolute paper-quality or standardized test instrument. Unlike the repository's acoustic, electronic, and instrumented mechanical fixtures, this uses optical transmission to compare paper crease damage.

## Design status
Reference design, not fabricated. RP2040 Pico module; no PCB layout and no KiCad schematic delivered. The connection list below is the schematic authority. Firmware core is host-buildable; target sensor acquisition adapter is not yet implemented. Do not interpret figures as validated performance.

## Block diagram
`USB 5 V -> Pico (3V3) -> ADC GPIO26 <- RC filter <- phototransistor divider <- LED-lit coupon`

## Connection list and power
Use Raspberry Pi Pico (RP2040 module), powered from USB 5 V. GPIO26/ADC0 measures the phototransistor collector through a 10 kΩ series resistor and 100 nF capacitor to ground; emitter to ground. A 3 mm white LED with 330 Ω series resistor from 3V3 to ground illuminates coupon; switch LED with GPIO14 via 2N7002 low-side MOSFET (gate 100 kΩ pulldown). GPIO15 is a normally-open trigger button to ground, internal pull-up enabled. USB connector is Pico's, not a separate power input. All grounds common. Do not connect external voltage above 3.3 V to GPIO.

Pico module pin mapping: ADC0 GPIO26 (physical pin 31), LED switch GPIO14 (pin 19), trigger GPIO15 (pin 20), 3V3 OUT pin 36, GND pin 38. No boot or flash pins are used. Peak current is USB/Pico dependent; LED current is approximately (3.3−2.8)/330 = 1.5 mA, assuming a 2.8 V white LED forward voltage. This is an estimate, verify actual LED VF and ADC range.

## BOM
See hardware/BOM.csv. The sensor is a generic phototransistor through-hole part: select one with visible response and check pinout. Enclosure and coupon-radius guide are user fabricated.

## Build / use
Host core test: `cmake -S firmware -B /tmp/crease-meter-build && cmake --build /tmp/crease-meter-build && ctest --test-dir /tmp/crease-meter-build --output-on-failure`.
Target operation is not implemented. Intended procedure: record a flat-coupon baseline, crease a matched coupon on a fixed-radius guide, align at the optical slot, acquire 64 ADC readings and compare median transmission against baseline. The included C routine converts ADC values to permille of baseline.

Example intended report: `baseline=2000 sample=1000 relative_transmission=500‰` (illustrative calculation, not a measured sample).

## Limitations
No target firmware, KiCad files, calibration procedure, repeatability data, or physical validation. Sensitivity depends on paper opacity, alignment, ambient light, LED geometry, and ADC noise. Use a light-tight shroud and matched uncreased control; this is a comparative workshop aid, not a standards-compliant test.
