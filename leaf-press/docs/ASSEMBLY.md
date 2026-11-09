# Assembly and bring-up

1. Confirm Pico module revision, GP14/GP15 labels, and USB connector orientation against its board documentation.
2. Fit MARK and NEXT normally-open buttons between the assigned GPIO and ground. Enable firmware pull-ups; optional 100 kΩ external resistors are not needed with these enabled.
3. Inspect for solder bridges. With buttons open, power from a current-limited USB source. Confirm the board's 3.3 V rail before attaching switches.
4. Attach switches and verify idle GPIO reads high and pressed reads low. Implement 20–40 ms debounce in the target adapter.
5. Connect USB CDC, send `START sample-001`, exercise MARK and NEXT, then confirm timestamps increase and session ends.

The included code is a portable event core, not complete Pico SDK firmware. Do not claim target operation until an SDK adapter and physical test are completed.
