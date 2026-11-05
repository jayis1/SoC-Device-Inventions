# Assembly and bench procedure

1. Use a Pico on a nonconductive bench and disconnect the cassette deck. Verify board pin numbering before wiring.
2. Confirm TCRT5000 LED/anode and transistor collector/emitter against its manufacturer datasheet. Do not assume a breakout module pin order.
3. Wire per README and schematic reference. Before USB power, check no short between 3V3 and GND.
4. Flash firmware, open USB CDC, and verify boot/reset behavior. With the head aimed at a stationary white card then black card, verify ADC counts differ and remain within 0–4095. Adjust standoff and ambient light shielding; record the range.
5. Test each button independently and verify event labels/time stamps. Repeat at least 20 presses; check for missed or repeated events.
6. Mount head in a smooth-edged noncontact guide with tape path clearance, turn the cassette manually only, and inspect the tape for any contact or scratching. Stop if the tape touches the fixture.

This procedure does not establish calibrated reflectance, material identification, cassette compatibility, or safe use in a powered deck. No physical test has yet been performed.