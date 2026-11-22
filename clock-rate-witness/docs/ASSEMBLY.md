# Assembly and bring-up

1. Assemble the Pico, optical sensor breakout, local 100 nF bypass and RUN switch on a breadboard/carrier.
2. With GP26 disconnected, power from USB and measure sensor AOUT on dark and reflective targets. Reject the breakout if output goes below ground or above 3.3 V.
3. Connect AOUT to GP26, button to GP15/GND. Keep the sensor stand clear of the clock hands.
4. Build the portable core with `cmake -S firmware -B /tmp/crw-build && cmake --build /tmp/crw-build`. A target Pico SDK USB/ADC adapter is not included.
5. Validate the optical edge threshold, reject double edges, and record at least ten minutes against a trusted reference. Preserve raw timestamps and note setup, target clock, and sample duration.

Do not mount anything on the moving hand that can impair the clock. The sensor is non-contact; use a removable marker only where acceptable.
