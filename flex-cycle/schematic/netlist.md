# Flex Cycle wiring schedule (reference)

Not a KiCad schematic. Build from these explicit connections; verify package pinout before wiring.

| Ref | Part/pin | Net | Connection |
|---|---|---|---|
| J1 | Pico VSYS | +5V_USB | USB-powered Pico board |
| J1 | Pico GND | GND | Common return |
| U1 | A3144 VCC | +5V_USB | Hall switch VCC |
| U1 | A3144 GND | GND | Hall switch return |
| U1 | A3144 OUT | HALL_N | Open-collector output |
| R1 | 10 kΩ | HALL_N to +3V3 | Pull-up; do not pull up to 5 V |
| C1 | 100 nF | HALL_N to GND | Local input filter |
| J2 | Pico GP2 | HALL_N | Hall event input, active low |
| SW1 | one contact | GP14 | Start/stop switch to GND |
| SW1 | other contact | GND | Active low, internal pull-up enabled |
| SW2 | one contact | GP15 | Reset switch to GND |
| SW2 | other contact | GND | Active low, internal pull-up enabled |

Use Pico castellated/module pads per the official Pico pinout. Suggested connectors: J1 USB micro-B on the Pico board; J2 three-pin keyed 2.54 mm (+5V/GND/OUT), confirm viewed mating-face orientation before crimping. Put R1/C1 near Pico GP2. The sensor output is open collector. Do not connect external 5 V logic to any Pico GPIO. Add 100 nF bypass between U1 VCC/GND at the sensor. This is a connection schedule, not a checked KiCad CAD file.
