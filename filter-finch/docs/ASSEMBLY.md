# Assembly and bring-up

## Prototype boundary

This guide builds a USB-powered bench/reference prototype from listed development boards. It is not a fabrication package: no production PCB, enclosure certification, KiCad design, EMC test, or HVAC listing is supplied.

## Mechanical installation

1. Power down and lock out the air handler according to its service instructions. Do not open electrical compartments.
2. Install one static-pressure tap immediately upstream of the filter and one immediately downstream, away from bends and fan discharge. Mark the tubes `FILTER +` and `FILTER -`.
3. Install the pitot probe in a straight duct section with its impact opening facing airflow; connect impact to `FLOW +` and static to `FLOW -`.
4. Route tubing outside the duct without kinks, low points that collect condensate, or contact with sharp sheet metal. Use grommets and strain relief.
5. Mount the electronics outside the duct and outside any mains compartment. The pressure sensors are not rated for wet, corrosive, combustible, or medically clean gas streams.

Do not drill into an energized enclosure. Before drilling a duct, verify both sides are clear of refrigerant lines, coils, wiring, fire dampers, and rotating equipment. Obtain permission where ducts are part of a rated fire/smoke system.

## Wiring

Use a common 3.3 V/GND rail and keep I2C wiring below 150 mm for the first prototype.

| ESP32-C6 | Connection |
|---|---|
| GPIO6 / GPIO7 | TCA9548A SDA / SCL, 4.7 kOhm pull-ups to 3.3 V |
| mux channel 0 | filter SDP810, address 0x25 |
| mux channel 1 | flow SDP810, address 0x25 |
| mux channel 2 | SHT45 0x44 and SSD1306 0x3C |
| GPIO4 / 5 / 2 / 3 | microSD SCK / MOSI / MISO / CS |
| GPIO10 | calibration button to GND, internal pull-up |
| GPIO11 | status LED through 1 kOhm |

Check each breakout's supply and logic levels against its own documentation. Do not assume similarly named breakout boards have identical pinouts.

## Bring-up

1. With all tubes open to the same room pressure, power from a current-limited USB source. Both differential readings should settle near zero; a large fixed offset indicates plumbing or sensor trouble.
2. Block neither sensor port. Apply a gentle pressure with a squeeze bulb, never compressed air, and confirm polarity independently on each channel.
3. Run the portable core tests from the repository README.
4. Install a new, correct filter. Run the fan at each normal speed for five minutes. Reject samples during speed changes and save at least five steady samples above 4 Pa pitot dynamic pressure.
5. Record the median normalized baseline. Label the baseline with filter model, orientation, fan mode, and date.
6. Compare the prototype against a calibrated low-pressure manometer before interpreting ratios. Repeat after tube routing changes.

## Service checks

Inspect taps and tubes monthly for condensation, dust, cracks, pinches, and disconnection. Relearn only after confirming that the filter, fan, dampers, and duct configuration represent the desired clean state. A relearn performed with a loaded filter hides the loading condition.
