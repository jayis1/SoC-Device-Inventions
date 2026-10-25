# Page Stepper verification plan

1. With USB disconnected, verify the two normally-open pedals each connect only their assigned GPIO to GND.
2. Build with Pico SDK 2.1.1 and inspect compiler warnings.
3. Flash the UF2 in BOOTSEL mode and verify enumeration as a USB keyboard.
4. In a text editor, confirm left/right presses send Page Up/Page Down once per stable press; verify bounce does not repeat.
5. Disconnect either pedal and verify the other still operates. Confirm no external voltage is present on either input.

No target toolchain, KiCad ERC, USB host test, or physical hardware test was available in this design run. Do not treat these steps as performed validation.
