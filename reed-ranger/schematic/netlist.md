# Reed Ranger wiring reference

Reference design only; this is not a native KiCad schematic or PCB. Connect by net name; all grounds are common.

| Ref | Connection |
|---|---|
| U1 Raspberry Pi Pico | USB connector supplies VSYS; GND common |
| J1 reed switch | pin 1 to GP2 through R1; pin 2 to GND |
| R1 1 kΩ | series between J1.1 and GP2 |
| C1 100 nF | GP2 to GND, physically near Pico |
| SW1 run/pause | GP14 to GND |
| SW2 clear | GP15 to GND |

Enable RP2040 internal pull-ups on GP2, GP14, GP15. Input is active-low. GP2 has no external pull-up; use Pico internal pull-up. Keep switch wiring short. No external voltage is permitted on GPIO.
