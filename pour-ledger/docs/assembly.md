# Assembly guide

1. Mount the load cell exactly as specified by its datasheet: one end fixed, the platform on the moving end; avoid side loading. Insulate strain-relief joints.
2. Identify bridge wires from the cell datasheet, not wire color assumptions. Connect E+/E-/A+/A- to HX711 module.
3. Wire modules using `schematic/netlist.md`; confirm HX711 logic voltage and OLED supply compatibility. Buttons short their GPIO to ground.
4. Before connecting the Pico, use a meter to check for shorts between 3V3 and ground. Power over USB with a current limit of 200 mA; verify 5 V VBUS and 3.3 V rail. Stop if current exceeds limit or any part heats.
5. Install Pico SDK 2.1.1 and build with CMake. Flash the generated UF2 by holding BOOTSEL while connecting USB, then copy UF2 to the RPI-RP2 volume.
6. At startup, tare unloaded. Calibration requires setting `SCALE_COUNTS_PER_GRAM` after recording raw counts unloaded and with a known reference mass; current source uses a placeholder coefficient and must not be used for accurate measurements as-is.
7. Verify the serial CSV and button behavior before placing a dripper. Keep electronics outside the spill area and never wash the assembly.

No physical hardware testing is claimed. Disconnect immediately for unstable rails, unexpected heating, or intermittent USB resets.
