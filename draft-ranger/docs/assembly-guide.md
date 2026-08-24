# Draft Ranger Assembly Guide

## 1. Mechanical Build

1. Print the handheld shell in ASA or PETG with a split-body design.
2. Install brass heat-set inserts for the shell, probe collar, and thermal-pad arm pivots.
3. Bond the stainless probe tube into the nose cone.
4. Route two 2 mm ID silicone tubes:
   - probe pressure line to SDP810 P+
   - room reference line to SDP810 P-
5. Install the acoustic dust mesh behind the probe-side microphone slot.
6. Mount the spring-loaded thermal pad arms and attach 0.5 mm silicone contact caps.

## 2. PCB Population Order

1. ESP32-S3 module
2. buck-boost regulator and charger
3. I2C sensors and RTC
4. ADS1115 + OPA2388 hot-wire front end
5. OLED header / display
6. microSD socket
7. buttons, buzzer, RGB LED
8. battery connector and probe harness

## 3. Hot-Wire Probe Construction

- Use a short removable probe cartridge so damaged wires can be replaced.
- Suspend the sensing wire across a ceramic or mica carrier.
- Keep the thermal mass low.
- Use twisted pair leads back to the bridge board.
- Add a guard hoop at the tip so the wire is not physically touched during scans.

## 4. Thermal Pad Construction

- Use copper tongues under Kapton for low-lag contact sensing.
- Electrically isolate the tongues from the user with conformal coat and silicone boots.
- Calibrate both TMP117 channels after final assembly because pad pressure changes offset slightly.

## 5. Bring-Up Checklist

- Verify no short between VBAT and 3V3.
- Power from bench supply limited to 250 mA first.
- Confirm 3.3 V rail before attaching display and battery.
- Scan I2C bus for SDP810, ADS1115, TMP117s, SHT45, DS3231, OLED.
- Check I2S microphone clocking.
- Enable probe heater briefly and confirm bridge responds without runaway.

## 6. Calibration Fixtures

Recommended fixtures:

- small variable-speed blower with reference nozzle
- narrow known-width crack plate
- chilled plate for condensation tests
- warm humidity chamber or salt packs for RH validation
- acoustic slit / whistle plate for tonal classification tuning

## 7. Field Use Tips

- Allow 60 s warm-up for the hot-wire probe.
- When scanning a long window edge, mark distance every 10 cm and press the mark button.
- For condensation diagnosis, sample indoor RH first, then surface temperatures.
- A leak with low pressure but strong whistle often indicates a very narrow gap.
- A leak with high pressure differential across many points may indicate building pressure imbalance rather than failed weatherstripping.
