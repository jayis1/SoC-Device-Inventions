# Flex Cycle

USB-powered cable-flex endurance counter for repeatable bench tests. A magnet on the moving fixture passes a Hall switch at each completed bend cycle; firmware counts qualified cycles, records session duration, and emits newline-delimited JSON over USB CDC. It is an observer/logger only: it does not drive the fixture or certify cable life.

## What is new

Unlike the repository's connector mating-force, hinge-cycle, and backlash fixtures, Flex Cycle counts repeated flex motions of a cable or small harness at a defined fixture position. It has no force sensor, angle measurement, or automated actuator.

## Hardware reference design

- MCU: Raspberry Pi Pico (RP2040 module; 3.3 V GPIO).
- Sensor: A3144E-compatible open-collector Hall switch, powered at 5 V; 10 kΩ pull-up to 3.3 V. Place a small permanent magnet on the moving fixture.
- Input: GPIO2 active-low, 100 nF from GPIO2 to ground for edge-noise suppression; firmware adds a 100 ms qualification window. Sensor output is open collector and must never be driven high at 5 V.
- User controls: GP14 start/stop active-low with internal pull-up; GP15 reset session active-low. USB CDC is the only data interface.
- Power: regulated 5 V USB into Pico VSYS; onboard 3.3 V rail supplies Hall output pull-up. Sensor supply uses USB 5 V. No battery, motor, mains connection, or wireless radio.
- Estimated current: Pico up to 100 mA typical workload plus Hall switch <10 mA; use a regulated USB supply rated 500 mA or more.

Pin assignments, connector polarity, and component list are in `schematic/netlist.md` and `hardware/BOM.csv`. The netlist is an explicit reference wiring schedule, not a native KiCad drawing. This is an unvalidated prototype design; no PCB, fabrication, endurance calibration, or physical test is claimed.

## Firmware

Pico SDK 2.1.1, C11. Configure `PICO_SDK_PATH`, then run `cmake -S firmware -B build -DPICO_SDK_PATH=$PICO_SDK_PATH`, `cmake --build build`; flash `build/flex_cycle.uf2` by BOOTSEL. USB CDC prints JSON lines. Sensor transitions inside the debounce window are ignored; counter saturates at UINT32_MAX, never wraps. Start/stop toggles the session. Reset clears the count and elapsed-time origin.

Example: `{"event":"cycle","count":42,"elapsed_ms":15980}`. Start/stop and reset events include their current count. On reboot the session begins stopped at zero; no persistent storage is used.

## Bring-up and limitations

Use a current-limited USB source. With the fixture stationary, verify GP2 reads high; each magnet passage should cause one count. Adjust sensor/magnet geometry to make exactly one qualified falling edge per full flex cycle. Validate against a manually counted 100-cycle run before using comparative results. The 100 ms lockout limits the maximum accepted cadence to 10 events/s. This design cannot determine bend radius, force, conductor damage, or fixture compliance and is not a product-life certification instrument.

See `docs/ASSEMBLY.md` and `docs/API.md`. Author: jayis1.
