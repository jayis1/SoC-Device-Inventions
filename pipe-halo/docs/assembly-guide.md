# Pipe Halo Assembly Guide

## 1. Mechanical Preparation

1. Print the clamp body in PETG or ASA with 0.2 mm layer height.
2. Bond the silicone contact pad into the piezo pressure finger recess.
3. Install brass heat-set inserts in the enclosure pod and clamp hinge.
4. Cut two stainless RTD strap bands to match the target pipe diameter.

## 2. PCB Assembly Order

1. Reflow all SMD passives and ICs on the 4-layer main board.
2. Solder the ESP32-C6 module last to minimize rework difficulty.
3. Hand-solder the OLED, terminal block, microSD socket, and JST field connectors.
4. Inspect the OPA320 piezo front-end for flux residue; leakage here can raise the noise floor.

## 3. Sensor Harnesses

### Piezo Sensor
- Route the piezo film with twisted pair or shielded cable.
- Add strain relief so clamp pressure does not peel the film traces.
- Use thin silicone foam between the metal pipe and piezo strip for consistent coupling.

### PT1000 RTDs
- Prefer 4-wire RTDs if cable length exceeds 20 cm.
- Apply a thin thermal grease pad under each RTD strap.
- Place RTD1 toward the supply side and RTD2 toward the fixture / branch side.

### Hall Valve Sensor
- Fix the Hall sensor PCB so its X/Y plane faces the rotating valve magnet.
- A 6 mm × 2 mm N52 magnet epoxied to the handle works well.
- Keep the magnet-to-sensor gap between 4 mm and 10 mm.

## 4. Mounting Recommendations

- Do not place Pipe Halo on unsupported flexible tubing where body movement dominates the vibration signal.
- For copper pipe, mount directly on clean metal.
- For PEX, place the sensor close to fittings or clamps where vibration couples better.
- For insulated pipes, cut a small inspection window in the insulation and reseal after installation.

## 5. First Power-Up

1. Power the board from a current-limited bench supply at 12 V, 250 mA limit.
2. Confirm 3.3 V rail and RTC backup voltage.
3. Flash firmware and open the UART console.
4. Verify all I2C devices enumerate and both MAX31865 channels return plausible temperatures.

## 6. Calibration

### Zero-Flow Baseline
- Keep all nearby valves closed.
- Record 60 s of quiet-pipe acoustic baseline.
- Save the RMS, centroid, and hammer baseline in NVS.

### Flow Baseline
- Open a faucet on the monitored line to three repeatable flow levels.
- Save thresholds for LOW_FLOW, NORMAL_FLOW, and HIGH_FLOW.

### Valve Angle Baseline
- Record Hall vectors at fully open, half open, and fully closed positions.
- Store the angle solution and allowed tolerance band.

### Freeze Baseline
- Compare RTD readings against an ice-water-validated probe.
- Adjust RTD offset if the absolute error exceeds ±0.4 °C.

## 7. Environmental Hardening

- Apply conformal coat for crawlspace, utility-room, or near-condensing installs.
- Use a desiccant vent or membrane vent if the enclosure is sealed.
- Keep the OLED and microSD side away from direct dripping paths.

## 8. Maintenance

- Re-check RTD strap tension every 6 months.
- Replace the backup cell every 2–3 years.
- Export logs quarterly to keep the microSD healthy.
- Re-run calibration after replumbing, valve replacement, or insulation changes.
