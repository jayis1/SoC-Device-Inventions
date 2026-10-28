# Darkroom Dose connection reference (not a KiCad schematic)

This is a functional topology, not a fabrication-ready circuit. Use a Raspberry Pi Pico board with its onboard USB/regulator; do not assemble a bare RP2040 without its required flash, clock, decoupling, and USB support.

## Power

- Pico VSYS/USB input: regulated USB 5 V only; rely on the Pico board's onboard 3V3 regulator.
- Pico 3V3(OUT) -> U1 OPA333 V+ and the 3V3 analog rail; add C1 100 nF directly at U1.
- All logic/sensor grounds connect to Pico GND. Keep the photodiode/TIA return local to U1 and join at one ground point.

## Photodiode transimpedance stage

- D1 BPW34 and U1 OPA333 form a photodiode transimpedance stage, but the bias polarity and feedback network are intentionally unspecified pending a checked reference design and bench validation. Do not wire this stage from this prose alone.
- U1 output -> Pico GP26/ADC0 (Pico header pin 31) through R2 1 kΩ; C2 10 nF from ADC node to ground is optional and unpopulated until stability is checked.
- This unresolved feedback-value/polarity detail is deliberately a hold point: validate OPA333 input common-mode and the desired output polarity experimentally before assembly. Do not treat this as a completed schematic.

## Controls and output

- Start and mode buttons connect GP6/GP7 respectively to ground; firmware enables internal pull-ups.
- GP8 -> R3 1 kΩ -> U2 LTV-817 LED anode; LED cathode -> ground. Add external pull-down 100 kΩ from GP8 to ground so reset defaults off.
- U2 transistor output is isolated and floating. Route only to a low-voltage SELV interface input, with that interface's own pull-up and protection. Do not connect this prototype to mains.
- I2C OLED header: 3V3, GND, GP4 SDA, GP5 SCL, 4.7 kΩ pull-ups to 3V3 (omit if the module already has pull-ups); address 0x3C.

## Required verification before use

Resolve U1 circuit polarity and feedback network using the OPA333 data sheet and photodiode operating mode. Check ADC input never exceeds 3.3 V. Validate optocoupler LED current and off-state leakage for the selected external interface. This reference has no PCB layout, no ERC, and no electrical isolation certification.