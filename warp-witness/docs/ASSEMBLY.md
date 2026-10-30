# Assembly

Prototype reference only, not a released PCB. Mount Pico, logic ICs, headers, and connectors on a carrier. Place 100 nF close to each IC. Wire the load cell only after checking its bridge leadout. Use USB for Pico/logic and a separate regulated motor 5 V supply of at least 1 A, with common ground. Do not power the motor from GPIO or 3V3.

Before connecting motor, verify 3.3 V logic and USB CDC. Verify motor stall current against the DRV8833 limit and thermal conditions; use a current-limited supply. Fit a capstan guard, slip clutch, and mechanical stop. Calibrate mass with known certified weights and length against a traceable ruler. Disconnect supplies before wiring.
