# Assembly and measurement procedure

1. Mount the Raspberry Pi Pico on a breadboard or carrier; do not connect USB yet.
2. Connect a 10 kOhm linear potentiometer across Pico 3V3(OUT) and GND; connect wiper to GP26/ADC0.
3. Connect two normally-open fixture orientation switches between GP2/GND and GP3/GND. The intended target adapter enables internal pull-ups. Both active at once is invalid.
4. Construct a rigid lever fixture with a repeatable 100 mm specimen overhang, pivot, and light spring. Keep geometry and preload unchanged between orientations.
5. Check wiper voltage by meter: it must remain within 0–3.3 V through full lever travel. Connect USB only after this check.
6. Establish empty-fixture and reference-strip readings. Insert a specimen to the mechanical stop; collect repeated means in orientation A, rotate it by 90 degrees, and repeat for B.
7. Compare repeated medians. The direction ratio is a relative indicator; it is not a modulus or calibrated force.

Keep the work low-voltage and dry. Do not connect powered machinery or mains contacts. Firmware target integration and fixture mechanics have not been tested.
