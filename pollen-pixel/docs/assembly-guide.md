# Pollen Pixel Assembly Guide

## 1. Print and Prepare Mechanical Parts

Print the enclosure, optical baffle, tape cassette, slit mount, and blower manifold in black PETG. Matte-black internal surfaces reduce flare during white and UV imaging.

Recommended printed parts:

- front shell with inlet slit pocket
- rear shell with blower duct
- camera tower with M12 lens thread
- tape cassette with 14 mm deposition band
- optical divider separating camera and blower cavity

## 2. Populate the PCB

Assemble in this order:

1. power tree: MCP73831, TPS63031, TPS7A2033, MT3608
2. ESP32-S3 module and decouplers
3. I2C/SPI support parts, RTC, OLED header, microSD socket
4. DRV8834 and stepper connector
5. SDP810 and pressure tubing barbs
6. camera FFC connector
7. MOSFET LED/blower switches
8. USB-C and buttons

Before adding the camera, verify:

- USB-C powers the board
- LiPo charging current is correct
- 3.3 V main rail and analog rail are within ±2%
- 5 V boost is stable under blower load

## 3. Optical Assembly

1. Mount HM0360 module beneath the M12 lens holder.
2. Install the white LEDs at 20–30° off-axis to suppress specular glare.
3. Install the 405 nm LEDs with a shallow excitation angle and black masks.
4. Set working distance to roughly 18 mm from lens to tape surface.
5. Lock focus using the supplied 10 µm calibration slide.

## 4. Tape Cassette

- Apply 14 mm wide low-fluorescence silicone adhesive tape to the cassette drum.
- Keep tape tension moderate; over-tension can skew step accuracy.
- Use the home flag so the firmware can re-zero drum position on boot.

## 5. Pneumatic Path

- Insert the stainless slit nozzle at the inlet.
- Connect short silicone tubes from the restriction block to the SDP810 high/low ports.
- Seal the blower path with foam tape or silicone gasket to avoid bypass leakage.

## 6. Firmware Bring-Up

1. Flash the ESP32-S3 base firmware.
2. Connect serial console at 115200 baud.
3. Run home-cycle command for the tape motor.
4. Run blower calibration against a reference flowmeter.
5. Capture dark, white, and UV flat-field frames.
6. Save calibration into NVS.

## 7. Recommended Calibration Targets

- 10 µm chrome dot calibration slide
- fluorescent polymer microspheres (8–20 µm)
- dust-free blank tape region for dark subtraction
- co-located reference pollen sampler for field correlation

## 8. Field Maintenance

- replace tape cassette every 1 to 7 days depending on loading
- clean slit nozzle weekly
- inspect blower for fiber buildup
- re-run focus and flow calibration monthly
- keep UV LED window clean; contamination strongly changes fluorescence contrast
