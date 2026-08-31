# Assembly Guide — Sump Shepherd

## 1. Mounting Strategy

1. Fix the enclosure to a wall or joist above the sump pit.
2. Drill a sealed hole in the pit lid for the **A02YYUW** ultrasonic sensor.
3. Aim the sensor vertically at the water surface with no float arm or pipe directly in its cone.
4. Route the CT clamp around **one insulated pump hot lead only**.
5. Attach the accelerometer side of the PCB enclosure firmly to the lid or discharge pipe bracket.
6. Run the leak rope around the pit perimeter and nearest floor low point.

## 2. Wiring

- **12 V IN**: from wall adapter or sump UPS accessory output
- **CT clamp**: to the analog input terminal block
- **Float input**: optional dry contact from legacy alarm float or reed float
- **Leak rope**: two-wire resistive rope or floor probes
- **UART ultrasonic**: 4-wire harness to the lid-mounted ranger

## 3. Commissioning

1. Power the device.
2. Pair over BLE or connect to the temporary Wi-Fi AP.
3. Set pit geometry:
   - lid-to-bottom distance
   - normal idle level
   - alarm level
4. Trigger one manual pump cycle.
5. Save the baseline current, vibration, and drawdown rate.

## 4. Placement Notes

- Keep the OLED side away from direct splash paths.
- Avoid mounting the ultrasonic sensor where the float or discharge stream enters the beam.
- Keep the SHT45 vent exposed to pit air, not sealed inside foam.
- If a check valve is loud, the accelerometer can be mounted on the discharge riser for better event clarity.

## 5. Maintenance

- Inspect the ultrasonic face every 6 months for condensation or cobwebs.
- Test the leak rope by touching it with a damp cloth.
- Review battery voltage monthly if the site experiences outages.
- Re-run baseline capture after pump replacement.
