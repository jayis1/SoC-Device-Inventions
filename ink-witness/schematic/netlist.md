# Wiring reference (not native KiCad)

| Ref | Connection | Notes |
|---|---|---|
| J1 Pico USB | USB host, 5 V | board power/programming |
| U1 Raspberry Pi Pico | GP14=J2.OUT; GP15=SW1; 3V3=J2.VCC; GND=J2.GND/SW1 | onboard regulator |
| J2 TCRT5000 digital sensor module | VCC=3V3; GND=GND; DO=GP14 | Verify module supports 3.3 V and active-low output; trim comparator threshold |
| SW1 RUN pushbutton | GP15 to GND | internal pull-up |

No external pull-ups, level shifter, battery, or mains connection. Sensor boards are not standardized; verify output voltage before connecting. Common ground required.
