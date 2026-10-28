# Morse Mate connection netlist

Status: authoritative textual netlist; no KiCad capture is supplied. Reference board: Raspberry Pi Pico (RP2040 module).

| From | To | Purpose / constraints |
|---|---|---|
| Pico VSYS | Pico onboard power circuit | USB 5 V input arrives through board USB connector; do not feed another supply simultaneously |
| Pico GND | Key contact 1 | Common ground |
| Pico GP15 | Key contact 2 | Normally open key; firmware enables internal pull-up; closed state is low |
| Pico GP14 | R1, 1 kΩ | Piezo signal current limit |
| R1 other end | Passive piezo + | Series drive; verify piezo voltage/current rating |
| Passive piezo − | Pico GND | Common ground |
| Pico SWD pads | Debug/programmer header (optional) | SWCLK, SWDIO, GND; preserve access |

Use only the Pico module's 3.3 V GPIO. The board's native USB connector provides CDC and power. No external pull-up, battery, charger, radio, or high-voltage circuit is fitted. This netlist has not received KiCad ERC or PCB layout review.
