# Selvage Scout wiring schedule (reference, not KiCad)

| Ref | Connection | Notes |
|---|---|---|
| J1 Pico | USB 5 V input | Pico USB connector; no external power |
| U1 Pico 3V3(OUT) | J2 TCRT5000 VCC | Module must support 3.3 V |
| J2 sensor GND | Pico GND | Common ground |
| J2 analog OUT | R1 1 kΩ → GP26/ADC0 | Analog output must stay within 0–3.3 V |
| C1 100 nF | GP26 to GND | At Pico input |
| SW1 | GP14 to GND | Internal pull-up, debounced in firmware |
| SW2 | GP15 to GND | Internal pull-up, debounced in firmware |
| Comb | Passive rounded nylon comb | Mechanically isolated, no electrical connection |

Do not connect sensor VCC to 5 V. Pico ADC maximum is 3.3 V; validate the module output with a meter before connecting. No PCB or electrical-rule-checked KiCad schematic is provided.
