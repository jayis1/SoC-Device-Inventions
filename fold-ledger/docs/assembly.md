# Assembly

This is a USB-powered prototype logger. No custom PCB is included; use a Pico and a small perfboard or a future PCB. Keep the optocoupler isolation barrier intact and treat only the Pico-side output as low-voltage logic.

1. Confirm the foot switch is normally open and connect between GP14 and GND.
2. Connect a 10 kΩ pull-up from GP15 to 3V3; connect the optocoupler collector to GP15 and emitter to GND.
3. Drive the optocoupler LED only from a separately isolated, SELV sensor output through a resistor selected for that source. Do not assume the unspecified source voltage.
4. Connect GP16 through 1 kΩ to the status LED and then GND.
5. Inspect continuity and check no external sensor-side conductor reaches Pico ground before attaching USB.
6. Flash the UF2 built with Pico SDK 2.1.1. Open the USB CDC port and verify a switch produces one `foot` JSON line after release; optical input produces `optical` on its active edge.

No industrial machine wiring, mains, or safety-critical use. The design has not been physically assembled or tested.
