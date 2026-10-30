Reference only; component values need bench validation.

USB 5V -> Pico VSYS; Pico 3V3 -> OLED VCC and sensor amplifier supply.
GPIO2 -> OLED SDA; GPIO3 -> OLED SCL. Common ground.
GPIO14 -> 2N3904 base through 1k; emitter GND; collector IR LED cathode; LED anode via 220R to 3V3.
Phototransistor collector -> 3V3 through 10k; emitter -> GND; sense node -> Pico GPIO15 (input with appropriate hysteresis conditioning).
Do not connect an unconditioned analog node as a digital input without checking threshold/noise. Add comparator with hysteresis for a robust build. GPIO16 buzzer via transistor. GPIO17 reset switch to GND with pull-up.
