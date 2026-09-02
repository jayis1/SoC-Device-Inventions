# Mold Scout Assembly Guide

## Mechanical Stack

1. **Front shell**
   - thin ABS or PC front skin over the guarded moisture electrode
   - recessed opening for the MLX90640 with IR window
   - two tactile button plungers
2. **Main PCB**
   - front edge contains the guarded moisture probe copper
   - thermal sensor isolated from regulator with cutout slots
   - OLED mounted on user side
3. **Battery compartment**
   - 18650 holder or flat LiPo pocket
   - foam isolation between battery and probe region
4. **Rear shell**
   - USB-C opening
   - vent slot near SHT45/BME688, but baffled from direct hand breath

## Assembly Order

1. Reflow all SMT parts except the OLED module and battery leads.
2. Clean the analog probe region thoroughly; flux residue shifts dielectric readings.
3. Solder the OLED header or flex adapter.
4. Mount the MLX90640 behind the IR window; avoid silicone outgassing near the lens.
5. Install the haptic motor with adhesive foam away from the thermal sensor.
6. Fit the battery and charger wiring.
7. Flash firmware and run baseline self-test.
8. Close the enclosure and re-run dry-air calibration.

## Probe Construction Notes

- use a **center plate + driven guard ring + grounded outer shield** geometry
- keep solder mask thickness consistent over the front electrode
- avoid exposed copper; cover with thin conformal dielectric film
- keep screw bosses and battery metal at least 12 mm behind the probe area

## Thermal Sensor Placement

- keep the MLX90640 field of view clear of the user’s fingers
- use matte black baffles around the window to reduce reflections
- do not place the buck-boost inductor within the sensor’s direct view

## Recommended Calibration Workflow

### 1. Air Zero
Hold the device in free air, away from walls, and store the complex impedance baseline.

### 2. Dry Reference Tile
Touch the included dry tile and store the dry-material transfer function.

### 3. Room Baseline
Scan a known-good wall section in the target room for comparative normalization.

### 4. Thermal Offset
Compare MLX90640 average against a matte black tape target at room temperature and store the offset.

## Field Use Tips

- let the device acclimate for 2-3 minutes after moving between indoors and outdoors
- do not interpret one hot shower spike in a bathroom as mold by itself
- scan a suspected area and a nearby control area built from the same material
- glossy tile and metal wallpaper can corrupt both dielectric and thermal readings

## Troubleshooting

| Symptom | Likely cause | Fix |
|--------|--------------|-----|
| Moisture reading high everywhere | hand coupling into probe | improve guard drive, hold device by rear grip |
| Thermal image drifts warm | regulator heating sensor | increase thermal isolation slotting |
| VOC index spikes after cleaning | solvent exposure | wait and rescan later, do not over-weight VOC alone |
| Scan quality rejected | too much motion | slow sweep, keep probe face flush to wall |
