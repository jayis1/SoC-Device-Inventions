# Connection-level schematic (design reference, not KiCad)

| Net | From | To | Notes |
|---|---|---|---|
| USB_5V | Pico USB connector VBUS | Pico onboard regulator input | Supply only; do not inject external 5 V |
| 3V3 | Pico 3V3 OUT | HX711 VCC, load-cell E+ | Keep module logic rail at 3V3 |
| GND | Pico GND | HX711 GND, load-cell E- | Common return |
| HX_DOUT | HX711 DOUT | Pico GP14 | Confirm output high <= 3.3 V |
| HX_SCK | Pico PD_SCK | Pico GP15 | Firmware keeps low except 25 clocks for channel A/gain 128 |
| BRIDGE_A+ | Load cell A+ | HX711 A+ | Differential signal pair |
| BRIDGE_A- | Load cell A- | HX711 A- | Differential signal pair |

Place 100 nF ceramic directly between HX711 VCC and GND if not already fitted on the module. The Pico board includes its own decoupling/regulator. Load-cell cable should be twisted/shielded; connect shield to GND at the electronics end only. Do not connect the module's E+/E-/A+/A- ambiguously: follow the actual load-cell color/code sheet and module silkscreen.
