# Assembly and bring-up

1. Mount both slot sensors in a stable bracket; sheet enters gate A then gate B.
2. Check each TCST2103 lead using its datasheet. Wire phototransistor collector through 10 kΩ to 3V3 and emitter to GND. Power IR LED through its own series resistor; 220 Ω is a starting value only and must be checked against supply, Vf, and the datasheet's current rating.
3. Verify no short between 3V3 and GND. Confirm outputs idle high and fall low when a card interrupts each slot.
4. Build and run the host core tests. A Pico SDK adapter is required for GPIO, monotonic clock, and CDC; the core alone does not create a UF2.
5. On a prototype, perform slow A→B and B→A card passes and verify event sequences. Keep fingers away from any nearby manual folder or press.

Hardware build, sensor polarity, EMC, timing accuracy, and mechanical repeatability are unverified. This reference does not specify a PCB or safety interlock.
