# Wheel Whisper conceptual schematic and placement reference

Status: text-based engineering reference, not a KiCad-native or ERC-cleared artifact. Designed by jayis1.

```text
J1 USB-C VBUS -- F1 --+-- U4 MCP73831 VDD
                      +-- D1 USBLC6 TVS to GND
J1 CC1,CC2 -- 5.1k -- GND; D+/D- are only routed if a selected charging/data implementation requires them.
U4 VBAT -- B1 protected LiPo -- U5 TPS62743 VIN
U5 VOUT 3V3 -- U1 nRF52840 VDD, U2 OPA320, U3 DRV5032, DISP1

PZ1 piezo -> RIN 1k -> U2 non-inverting input; U2 is biased at 1.5V.
U2 output -> R 1k -> U1 P0.02/AIN0. Protect and limit the analog input to 0..VDD.
U3 DRV5032 OUT -> U1 P0.11; U3 VCC=3V3, 100nF local bypass.
U1 P0.26/P0.27 -> DISP1 SDA/SCL; R1/R2 4.7k to 3V3.
U1 P0.20 -> Q1 gate; Q1 low-side switches M1 motor. Keep motor return at battery star.
U1 P0.06/P0.07 -> SW1/SW2 -> GND. U1 P0.13 -> R5 1k -> LED -> GND.
U1 P0.31/AIN7 <- divided VBAT, divider and clamp selected to keep AIN <= VDD.
SWDIO/SWCLK test pads -> U1 P0.18/P0.19.
```

## Required layout review

- Use the exact nRF52840 reference layout, 32 MHz crystal network, decoupling, RF matching, and antenna keep-out for the selected module or chip design.
- Do not share a motor return or piezo return with the RF/analog reference path.
- Keep the piezo front end inside a grounded guard region; provide a resistor and clamp suitable for the expected tap transient.
- Use a protected, certified cell and validate charge current/thermal behavior. Do not charge unattended.
- Verify USB-C, ESD, battery protection, footprint library mapping, creepage, and enclosure strain relief before fabrication.
