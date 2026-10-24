# Reference netlist (not KiCad)

This human-readable connectivity description is authoritative for this prototype; no KiCad schematic or PCB has been created. Pico headers are module interfaces; validate exact header numbering against the chosen Pico board revision before assembly.

| Net | Connections | Notes |
|---|---|---|
| VBUS_5V | USB input, Pico VBUS | Pico USB-powered only |
| VSYS | Pico VSYS | Onboard power path |
| +3V3 | Pico 3V3_OUT, sensor VCC, pull-up resistors | Do not use 5 V sensor modules |
| GND | Pico GND, both sensors, buzzer return, LED return | Common ground |
| CYCLE_N | Pico GP2, 10k pull-up to 3V3, sensor 1 open-collector output | Falling-edge pulse |
| FEED_N | Pico GP3, 10k pull-up to 3V3, sensor 2 open-collector output | Falling-edge pulse |
| BUZZER_SW | Pico GP14 -> 100R -> 2N7002 gate; gate 100k to GND | Source GND, drain buzzer negative; buzzer positive 3V3 |
| STATUS | Pico GP15 -> 1k -> LED anode; LED cathode GND | Active high |

Add local 100 nF decoupling at each sensor. The reflective modules often include comparators and trim pots; their output stage varies. Confirm output voltage/polarity with a meter before connecting. Keep wiring and sensor brackets clear of moving machine parts.
