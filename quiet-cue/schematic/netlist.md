# Connectivity reference (not a KiCad schematic)

Power: J1 USB VBUS/GND connect to Raspberry Pi Pico VBUS/GND. Pico 3V3_OUT supplies U1/U2 VCC and encoder pull-ups. GND is common.

- J2 encoder A -> GP2; R1 10 kΩ from GP2 to 3V3.
- J2 encoder B -> GP3; R2 10 kΩ from GP3 to 3V3.
- J2 switch -> GP4; R3 10 kΩ from GP4 to 3V3; switch other terminal -> GND.
- Pico GP6 -> U1 SER; GP7 -> U1 SRCLK and U2 SRCLK; GP8 -> U1 RCLK and U2 RCLK.
- U1 QH' -> U2 SER. U1 QA-QH -> R4-R11 -> D1-D8 -> GND.
- U2 QA-QB -> R12-R13 -> D9-D10 -> GND.
- U1/U2 OE# -> GND; SRCLR# -> 3V3. C1/C2 100 nF from each IC VCC to GND.

Check SN74HC595D pin numbering and the Pico module connector against their current datasheets; this logical connectivity description is not validated by CAD/ERC and is not a PCB netlist.
