# Chill Warden Assembly Guide

## 1. Safety Notes

Chill Warden is a **diagnostic accessory**, not a permanent line-powered appliance.

- attach probes only to insulated and mechanically stable points
- keep hands clear of moving fan blades and hot discharge lines
- never route CT leads where they can abrade on sheet metal
- verify magnets will not interfere with nearby control devices or loose fasteners
- the optional thermostat-call input must be isolated through the on-board optocoupler only

## 2. Recommended Build Order

1. Populate USB-C, charger, buck-boost, battery connector, and fuel gauge.
2. Bring up the ESP32-S3 service console.
3. Populate I2C devices: OLED, SHT45, TMP117s, ADS122C04, IMU.
4. Populate the piezo front end and CT rectifier path.
5. Add the I2S MEMS microphone and verify clock integrity.
6. Wire the external clamp probes and CT jack.
7. Install battery, gasket, and magnetic rear plate.

## 3. Probe Construction

### Suction Clamp

- use a miniature insulated copper jaw clamp
- embed TMP117 on a small flex or thin FR-4 tongue
- add silicone grease pad for repeatable thermal coupling
- insulate the clamp body with heat-shrink and Kapton

### Discharge Strap Probe

- mount TMP117 on a narrow stainless-backed flex tongue
- add fiberglass sleeve for hot-line contact protection
- secure with a worm-drive or spring band clamp

### Piezo Shell Foot

- bond the piezo disc to an aluminum puck with silicone RTV
- mount the puck to the enclosure foot through a compliant isolator
- this improves shell-borne knock pickup while reducing airborne acoustic contamination

## 4. Calibration Flow

### Temperature

- clamp both probes to a copper pipe jig at 5 °C and 60 °C
- record per-probe offset
- confirm probe-to-probe mismatch stays below ±0.3 °C

### Current

- clip CT around a known AC current source at 2 A, 5 A, and 10 A
- fit a scale factor from ADC counts to RMS current proxy

### Vibration / Piezo

- attach to a quiet compressor shell and record baseline
- attach to a worn fan motor or shaker source and verify alert threshold separation

### Acoustic

- measure fan-on / fan-off SPL proxy and verify at least 6 dB separation

## 5. Field Deployment Tips

- place the main enclosure away from the condenser exhaust stream if possible
- keep the ambient sensor shaded from direct sun
- strap the discharge probe at least 100 mm downstream of the compressor stub when safe
- on heat pumps, log through a full defrost cycle when troubleshooting winter faults

## 6. Maintenance

- inspect clamp jaw insulation monthly in service fleets
- replace silicone thermal pads when contaminated with oil
- recharge the battery after each full survey day
- re-run temperature calibration every 12 months
