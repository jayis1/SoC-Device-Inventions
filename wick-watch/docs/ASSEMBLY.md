# Assembly guide

1. Mount Pico so USB remains accessible. Build a rigid vertical rail with five sensor modules at measured offsets of 10 mm. Mark each sensor's optical axis and use a clip that holds the sample face at a repeatable distance.
2. Wire sensor DO outputs to GP2..GP6 and module VCC/GND to Pico 3V3/GND. Check every output with a multimeter before connecting. A digital module whose pull-up is attached to 5 V is not compatible; replace it or rework its pull-up.
3. Wire normally-open SW1 from GP14 to GND. Wire GP15 through 1 kΩ and LED to ground. Verify continuity and absence of shorts before USB power.
4. Adjust each sensor threshold with dry and wet sample surfaces; the firmware expects wet=logic low and dry=high. Ensure no false transition from ambient light.
5. Build and flash per README. Open serial, verify header, press and release start, then perform a test with a strip and dyed water.
6. Keep spills away from electronics. The reservoir is separate and below the sensor rail; wipe the sample holder between runs.

This is an unbuilt reference design. No measured accuracy, repeatability, or safety certification is claimed.
