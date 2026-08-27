# Line Shepherd Assembly Guide

## 1. PCB Stackup and Layout Priorities

1. Use a 4-layer PCB: signal / ground / power / signal.
2. Keep the launch bridge, THS3201, comparator, and mux within a tight analog island.
3. Route the pulse-launch trace as short and straight as possible.
4. Separate the ESP32-C3 antenna edge from the comparator and launch network.
5. Place TVS and PTC parts physically near the external connectors.

## 2. Build Order

1. Populate the power tree first: MCP73831, TPS63031, TPS61023, TLV75533.
2. Verify VBAT, VSYS_3V3, ANA_3V3, and PULSE_5V before installing the MCUs.
3. Install STM32G474 and confirm SWD programming.
4. Install the ADG1608, THS3201, ADCMP600, and INA186 front end.
5. Add OLED, buttons, buzzer, microSD, and RTC.
6. Install ESP32-C3 last to avoid accidental RF-module heat stress during rework.

## 3. Bring-Up Sequence

- Confirm idle current draw with display disconnected.
- Sweep the comparator threshold DAC and confirm clean transition on a bench pulse.
- Inject a known 10 ns to 100 ns pulse and validate timer capture offset.
- Check foreign-voltage detect by applying 5 V, 12 V, and 24 V through a protected source; ensure launch lockout triggers above threshold.
- Validate the loop-resistance path with 10 Ω, 47 Ω, 100 Ω, and 1 kΩ standards.
- Validate capacitance timing with 1 nF, 10 nF, and 47 nF reference capacitors.

## 4. TDR Calibration Fixtures

Prepare simple calibration standards:

- 10 m open cable
- 25 m shorted cable
- 50 m cable with midpoint branch tap
- 30 m cable with an intentionally bad IDC splice
- moisture-simulated cable section using a damp leakage harness in a controlled lab fixture

Store the launch delay and velocity-factor corrections in flash per cable family.

## 5. Enclosure Recommendations

- Use a two-compartment enclosure: analog front end near connectors, digital/radio zone near battery and OLED.
- Provide finger-safe recessed banana jacks.
- Add a removable rubber boot for field use.
- Label pair selections clearly for RJ45 testing.

## 6. Field Use Tips

- Never connect to energized mains, phone ringing lines, or unknown outdoor plant without checking lockout status first.
- Start with the lowest-energy pulse setting on short indoor cables.
- For long irrigation or fence wiring, use wider pulses and average several shots.
- Record the actual cable type and known length whenever possible; velocity-factor errors dominate distance uncertainty.
