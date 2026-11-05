# Assembly and acceptance

1. Keep Pico and wiring above the tank waterline; provide drip loops and strain relief.
2. With USB disconnected, wire switch from GPIO15 to GND, LED from GPIO16 through 330 ohms to GND.
3. Inspect for shorts. Plug into a current-limited USB source; confirm LED behavior follows float switch.
4. Build and flash per ../README.md. Open USB CDC and confirm initial JSONL state record.
5. Operate switch repeatedly, including brief chatter; stable transitions should be emitted after 50 ms. Check timestamps are monotonic within one boot.
6. Power-cycle and confirm fresh initial-state record; timestamp restarts at zero.

Acceptance is a basic electrical/firmware smoke check only. Verify mounting threshold with a ruler and manual water level before use. No unattended top-off, leak protection, or hardware validation is claimed.
