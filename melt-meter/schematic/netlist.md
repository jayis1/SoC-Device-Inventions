# Melt Meter wiring reference (not a KiCad schematic)

U1: Raspberry Pi Pico SC0915. USB VBUS powers U1 only.
U2: MAX31855K (3.3 V supply); decouple VCC-GND with C1=100 nF close to U2.
J2: Type-K miniature thermocouple connector, T+ to T+ and T- to T-. Use matching Type-K extension wire; polarity is marked at the connector.
U1 3V3(OUT) -> U2 VCC, R1 top, C1 top.
GND -> U2 GND, C1 bottom, thermocouple cold-end reference only as defined by converter; do not ground the measurement junction.
GP2/SPI0_SCK -> U2 SCK.
GP4/SPI0_RX/MISO <- U2 SO.
GP5 -> U2 CS; R1=10 kΩ from CS to 3V3 keeps converter deselected during reset.
GP3/MOSI unused. GP25 is onboard LED.

The MAX31855 is read-only, SPI mode 0, 32-bit frame. Decode D[31:18] as signed 14-bit thermocouple temperature at 0.25 °C/LSB; D[16] is general fault and D[2:0] identifies open/short faults. Consult the exact datasheet revision before implementation. Keep probe electrically isolated from energized heater parts. Wiring reference only: no KiCad project, PCB, ERC, or fabrication validation supplied.
