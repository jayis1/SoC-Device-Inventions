# Assembly and safe first use

1. Build the fixture with the M6×0.5 screw parallel to the load-cell axis. The moving platen must be guided so it does not apply side load to the cell.
2. Mount the load cell according to its datasheet: one end fixed, force applied at the specified loading point. Fit a flat, centered switch platen and a replaceable switch socket.
3. Fix a diametric magnet concentrically to the screw shaft and mount the AS5600 so its sensing face is aligned to the magnet per the sensor datasheet. Mounting is sensitive to air gap and centering.
4. Wire only after checking each board's labels and supply limits. Verify no 5 V signal reaches Pico GPIO. Confirm I2C address 0x36 and that the breakout does not already include pull-ups before adding R1/R2.
5. With the USB disconnected, inspect for shorts and confirm the screw travels freely. Connect USB, check 3V3-to-GND rail first, and confirm no unexpected heating. USB current-limit protection is not a substitute for a bench supply during initial prototype bring-up.
6. Open USB CDC at 115200 baud (line rate is informational for Pico stdio), tare the logger externally, and apply a known small mass to establish a force conversion. Never exceed 1 kg or side-load the cell.
7. Install a switch, move the screw slowly through the stroke and back, and retain both directions. Stop immediately if the mechanism binds, the cell bends, wiring heats, or readings saturate.

The device is a low-voltage prototype. It is not calibrated, certified, or intended for safety-critical decisions. Keep fingers clear of the moving platen and pinch points.
