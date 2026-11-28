# Serial API

USB CDC console, newline-delimited ASCII, 115200 baud setting (USB CDC ignores physical baud).

Commands:

- `L <mm>`: set specimen length from 10 through 10000 mm.
- `S`: begin count when valid length is set and no capture is active.
- `E`: finish and print a record; alternatively press the GP14 capture button.

Output starts with `CSV,turns,length_mm,turns_per_km`. Each completed record is `DATA,<turns>,<length_mm>,<turns_per_km>`. This fixed unit is turns/km. Inputs are bounded; malformed lengths and invalid command/state combinations return `ERR`. Records are session-only and not persisted after power loss. Gate edges are rising-edge events; a 2 ms dead-time suppresses bounce/noise. That threshold is an unvalidated design assumption and can miss faster turns.
