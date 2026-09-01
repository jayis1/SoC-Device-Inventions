# Tensi Bead Assembly Guide

## Mechanical stack

1. Print or machine a dark ABS optical chamber with three regions: syringe tower, sample window, and contact-angle platen.
2. Mount the **ESP32-S3 main PCB** on brass standoffs above the battery tray.
3. Fit the **HM0360 camera board** so its optical axis is normal to the backlit window; keep lens-to-drop distance near 34 mm.
4. Install the **diffuse white LED backlight** opposite the camera with a 1 mm opal acrylic diffuser to produce a clean silhouette.
5. Bolt the **NEMA8 syringe stage** to the top tower, driving an M5x0.8 lead screw through a flexible coupler.
6. Add the **5 kg mini load cell** under the removable platen so sessile-drop test coupons can be weighed and zeroed.
7. Route the **SHT45** into a vented side pocket away from LED self-heating.
8. Place the OLED behind the front panel and expose USB-C, microSD, and the refill syringe hatch.

## Electrical bring-up

1. Verify 5 V from USB-C or battery boost input.
2. Verify 3.3 V buck output before inserting the ESP32 module.
3. Check I2C pull-ups and confirm SHT45 / OLED / TMP117 addresses.
4. Confirm camera XCLK and SCCB communication.
5. Jog the syringe stage at low current before loading any liquid.
6. Tare the load cell with empty platen and with a reference 10 g mass.

## Optical calibration

1. Insert a 5.00 mm calibration pin in the sample window.
2. Capture 16 frames and average the pixel diameter.
3. Store `pixel_scale_mm = 5.00 / measured_pixels` in NVS.
4. Repeat after any camera refocus or window replacement.

## Surface-tension calibration

- Run distilled water at 20-25 C as the primary check: expect ~72 mN/m.
- Run isopropyl alcohol as a low-tension check: expect ~21-24 mN/m.
- If both are offset in the same direction, re-check optical scale and drop-volume repeatability.

## Contact-angle calibration

- Use clean borosilicate glass as a low-angle standard.
- Use a PTFE coupon as a high-angle standard.
- Verify left/right contact angles agree within ±3°.

## Safety

- The syringe stage can pinch fingers; home it at low speed after assembly.
- Clean unknown liquids in a fume hood if volatile or corrosive.
- Use chemically compatible tubing and discard contaminated needles.
