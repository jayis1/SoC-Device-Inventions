# Assembly and bring-up

1. Mount the optical interrupter so its output changes once per slot; connect only after confirming its board's output voltage is 3.3 V. Fit a 100 nF bypass capacitor at the sensor.
2. Wire U1, OLED, MARK, and piezo stage exactly as in `../schematic/netlist.md`. Verify continuity and ensure no GPIO can see 5 V.
3. Power U1 from USB only. Measure 3V3 to GND before attaching peripherals. Confirm OLED address 0x3C using an I²C scanner.
4. Connect the sensor outputs and rotate the wheel by hand. Confirm both channels toggle and direction is stable; this prototype firmware core does not yet include target GPIO/ display drivers.
5. Build and run host core tests as documented in the device README. Do not infer wheel accuracy from a successful host test.

## Measurement validation

Compare at least 5 m against a tape on packed and loose soil; record signed error. Calibrate the effective circumference in the target conditions. No fabricated unit, environmental test, or calibration data is claimed.
