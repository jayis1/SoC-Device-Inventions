# Connection specification (not a KiCad schematic)

This connection table specifies a Pico module plus a verified 3.3 V-compatible HX711 breakout (BOM U2); it is not a native KiCad capture, PCB layout, or fabrication package. Choose a breakout exposing VCC, GND, DOUT, SCK and four bridge terminals; check its datasheet for signal levels and RATE strap. No bare-chip HX711 footprint is defined here.

| Net | Nodes | Notes |
|---|---|---|
| USB_VBUS | Pico USB input | USB 5 V powers the Pico board only |
| +3V3 | U1 3V3(OUT), U2 VCC | Only if breakout's documented supply range supports 3.3 V |
| GND | U1 GND, U2 GND, LC1 E− | Common ground; fixture metal not bonded by default |
| HX_DOUT | U2 DOUT, U1 GP2 | Must remain 3.3 V logic; breakout must not pull to 5 V |
| HX_SCK | U2 SCK, U1 GP3 | Firmware supplies 25 clocks/read, channel A gain 128 |
| HX_RATE | U2 RATE strap to GND | Configure module for 10 SPS; check breakout documentation |
| LC_E+ | U2 E+, LC1 excitation+ | Follow cell datasheet and HX711 breakout wiring diagram |
| LC_E− | U2 E−, LC1 excitation− | Follow cell datasheet and HX711 breakout wiring diagram |
| LC_A+ | U2 A+, LC1 signal+ | Cell wire colors are not standardized |
| LC_A− | U2 A−, LC1 signal− | Cell wire colors are not standardized |

The selected module must have its own local supply decoupling and known bridge excitation arrangement. Confirm module terminal labels and voltage compatibility before connection; no specific breakout model has been selected or validated.
