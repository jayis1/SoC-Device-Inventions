# Assembly notes

1. Use a Raspberry Pi Pico and the referenced low-voltage components; U1 is a module, not a bare RP2040 circuit.
2. Wire the divider and LED exactly as `../schematic/connections.md`; verify there are no shorts from 3V3 to ground.
3. Connect sensor node only to GP26/ADC0. Never allow it above 3.3 V. The Pico is powered from USB.
4. Place Q1 facing the moving pendulum/balance and shield from ambient light. This physical arrangement is experimental and may need an aperture or reflective target.
5. Target integration is not included. Feed debounced/hysteretic threshold crossings, timestamped in microseconds, to `beat_window_crossing`.
6. Host-test the core using the command in `../README.md`. A known-period LED source can exercise the optical setup; no accuracy claim is made.

No fabrication, safety, or physical-performance validation has been completed.
