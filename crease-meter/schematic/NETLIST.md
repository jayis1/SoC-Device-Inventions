# Wiring reference (not a KiCad drawing)

Pico 3V3 -> R1 330R -> D1 LED anode; LED cathode -> Q1 drain; Q1 source -> GND; GPIO14 -> Q1 gate; R3 100k gate-to-GND. Sensor: 3V3 -> phototransistor collector; emitter -> ADC node; ADC node -> R2 10k -> GPIO26/ADC0. C1 100nF from GPIO26 side to GND. SW1 from GPIO15 to GND (enable internal pull-up). Pico GND is common. Optical slot shields the sensor from ambient light. Verify chosen phototransistor collector/emitter pinout before assembly.

No KiCad project or PCB is supplied; this connection list is intentionally identified as a reference, not a fabricated schematic.
