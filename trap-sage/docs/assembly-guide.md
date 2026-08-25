# Trap Sage Assembly Guide

## 1. Mechanical Subassembly

1. Print or machine the clamp body halves.
2. Install the torsion spring and verify repeatable closure force.
3. Bond the copper jaw shoes into their recesses with thermally conductive epoxy.
4. Add 0.5-1.0 mm silicone thermal interface pads over the TMP117 pockets.
5. Bond the 27 mm piezo disc to the upstream jaw using a thin rigid epoxy layer.
6. Route the piezo leads with strain relief and foam isolation.

## 2. PCB Assembly

1. Assemble the main PCB as a 4-layer board.
2. Populate power supplies first: MCP73831, TPS63031, TLV75533.
3. Verify 3.3 V digital and analog rails before installing the ESP32 module.
4. Populate the OPA1652 + PCM1808 ultrasonic path and confirm no oscillation.
5. Install the OLED, buttons, haptic driver, and microSD socket.
6. Solder the TMP117 jaw flex leads last to avoid pad damage.

## 3. Bring-Up Sequence

- Check USB charging current limit.
- Verify deep-sleep current before connecting the OLED.
- Confirm I2C detection of TMP117 x2, SHT45, DS3231, ADS1115, and DRV2605L.
- Inject a 20 kHz sine into the AFE and verify I2S amplitude capture.
- Squeeze the clamp and confirm FSR response changes monotonically.

## 4. Thermal Calibration

- Clamp both jaws to the same heated pipe simulator block and store offset.
- Repeat at ~40 °C, ~90 °C, and ~140 °C.
- Save per-jaw offset coefficients in NVS.

## 5. Acoustic Calibration

- Use a bonded shaker puck or reference ultrasonic exciter.
- Capture known amplitudes at 12 kHz, 20 kHz, 32 kHz, and 40 kHz.
- Adjust gain jumper/pad option so normal traps do not clip while blow-through remains in range.

## 6. Field Use Tips

- Always seat the clamp on clean metal, not thick paint or insulation.
- Measure upstream and downstream on the same trap body orientation each route.
- Log several seconds of data before classifying a cycling trap.
- Use the cavitation mode on bypass valves or condensate throttling valves when the line is not at steam temperature.
