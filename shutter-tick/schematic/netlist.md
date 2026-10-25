# Connection netlist (reference, not KiCad)

- J1.1 -> Pico 3V3(OUT); J1.2 -> GND; J1.3 -> GP15.
- Q1 emitter -> GND; Q1 collector -> GP15.
- R1 10 kΩ from GP15 to 3V3(OUT). Illumination raises the phototransistor current and pulls the signal low; firmware detects polarity configured for the selected hood geometry.
- C1 100 nF between 3V3 and GND near Q1.
- D1 anode -> GP25 through R2 1 kΩ; D1 cathode -> GND.
- USB connects only to the Pico board. No external power connector.

Check phototransistor response and logic polarity on the assembled sensor before relying on duration values. The detector is an optically coupled circuit; no galvanic connection to the camera.
