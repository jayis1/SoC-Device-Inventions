# Melt Meter

Melt Meter is an offline, USB-powered thermal-profile logger for hobbyist solder-paste reflow on a small hot plate. It records one Type-K thermocouple at 4 Hz and exports timestamped Celsius readings over USB CDC; it does not control a heater or certify a lead-free profile. Intended for low-voltage benchtop characterization under continuous human supervision.

## Architecture

```text
USB 5 V -> Raspberry Pi Pico (RP2040)
Type-K probe -> MAX31855K (3.3 V SPI) -> Pico SPI0
Pico USB CDC -> timestamped CSV
```

Pico SC0915 module: GP2=SCK, GP3=MOSI (unused; MAX31855 is read-only), GP4=MISO, GP5=CS; GP25 onboard status LED. MAX31855K cold-junction compensated converter uses 3.3 V. Use a grounded, insulated-bead probe and keep its tip electrically isolated from the plate and mains-powered equipment. The SPI data-ready/fault bits are checked; open/short probe faults produce `FAULT` records, never invented temperatures.

## Pins and wiring

| Pico signal | MAX31855 breakout | Notes |
|---|---|---|
| 3V3(OUT) | VIN | Confirm breakout accepts 3.3 V; bare IC supply is 3.0–3.6 V |
| GND | GND | Common logic return |
| GP2 / SPI0 SCK | SCK | SPI mode 0, 4 Hz reads |
| GP4 / SPI0 MISO | SO | 32-bit frame |
| GP5 | CS | 10 kΩ pull-up to 3V3; deselected at reset |
| GP3 | NC | MOSI not connected |
| USB | host | Power + CDC serial |

Probe polarity follows the probe manufacturer's markings; MAX31855 open/short detection is not a substitute for checking reversed or mislocated probes. Keep thermocouple wiring twisted and away from heater switching leads.

## Power and BOM

USB supplies only the Pico and low-current converter breakout. No battery, radio, heater drive, or mains connection exists. U1 is the Raspberry Pi Pico SC0915; U2 is a MAX31855K cold-junction thermocouple breakout or bare MAX31855KASA+ with local 100 nF decoupling and correctly fitted SOIC-8 adapter; J2 is a Type-K miniature connector; R1=10 kΩ CS pull-up; C1=100 nF at U2. Breakout boards vary; verify pin labels, onboard regulation, and logic levels before connecting. No PCB fabrication files are supplied.

## Firmware and protocol

Portable C core in `firmware/src/melt_meter.c`; target adapter is documented in `firmware/README.md`. CSV header: `elapsed_ms,temp_c,status`. Status is `OK` or `FAULT`; fault rows leave temp_c empty. Sampling is scheduled at 250 ms without blocking sensor conversions. SDK-dependent Pico USB/SPI adapter is not included, so this is a firmware core reference, not a flashed application.

Host check:

```sh
cc -std=c11 -Wall -Wextra -Werror -I firmware/include -c firmware/src/melt_meter.c -o /tmp/melt_meter.o
python3 scripts/check_csv.py
```

## Assembly and test

Follow the netlist in `schematic/netlist.md`; keep the Pico USB-connected only to an isolated host while measuring. With the probe at room temperature, compare against a trusted thermometer, then record a supervised low-temperature warm-up and cool-down. Test unplugged probe handling before using the unit. Never leave a hot plate unattended; do not touch hot probe tips. This device neither switches nor disconnects heater power. No physical validation, calibration, target firmware build, KiCad ERC, or thermal accuracy characterization is claimed.

## Limits and license

Reference design only, not a safety interlock, calibrated instrument, or solder-joint acceptance tool. Sample rate and absolute accuracy depend on converter, probe, wiring, and placement; users must consult the MAX31855 datasheet and heater manufacturer's safety instructions. MIT. Authored by jayis1.
