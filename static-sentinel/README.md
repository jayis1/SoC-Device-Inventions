# Static Sentinel — ESD Workstation Readiness Monitor

> A USB-powered, low-voltage reference design that periodically checks an ESD mat and a personnel wrist strap, records environmental conditions, and shows an actionable local status before sensitive electronics work begins.

## Status and scope

| Evidence | Status |
|---|---|
| Product/architecture design | Complete reference design |
| Firmware core | Host-built and unit-tested |
| KiCad ERC / PCB layout | Not run / not supplied |
| Physical prototype | Not assembled or bench-tested |
| Calibration | Required with traceable resistance standards |

Static Sentinel is a **SoC-based device invention**, not a custom semiconductor. It is not an ANSI/ESD S20.20 compliance instrument, a continuous personnel monitor, a certification tool, or evidence that a workstation is safe. Validate the circuit, enclosure, calibration, and local ESD-control policy before relying on it.

## User problem and novelty

A technician setting up a repair, small-run assembly, or classroom electronics bench often notices a disconnected strap or a dried-out mat only after a component failure. Existing inventions in this repository focus on environmental, scientific, agricultural, building, and machine diagnostic instruments. Unlike the closest environmental-monitoring designs, **Static Sentinel** enables a different outcome: a local go/no-go ESD-workstation check through low-voltage resistance measurement of bench-control accessories, rather than measuring an ambient condition alone.

The intended first-use success criterion is simple: with known-good 1 Mohm standards connected at the mat and strap inputs, the screen/serial record reports `OK`; an open, short, or out-of-policy standard reports the matching fault.

Non-goals: ionizer balance, high-voltage charge measurement, automated compliance reporting, remote control, and any determination of component damage.

## Architecture

```text
USB-C 5V -> PTC fuse + TVS -> AP2112K 3V3
                                  |
                           ESP32-C6-WROOM-1-N8
                         /       |        \
                  I2C GPIO6/7  GPIO2      GPIO3/4/5
                     /     \       |          |
                 SHT45    ADS1115 1 Mohm test drive  buzzer / LED / button
                           /     \
                    mat terminal  wrist-strap terminal
```

The ESP32-C6 provides local control and optional future Wi-Fi/BLE integration. Firmware defaults to no credentials, no telemetry, and no external network action. The measurement connector is intentionally limited to 3.3 V through 1 Mohm resistance. It must never be connected to mains, an ionizer output, charged conductive tooling, or any external source above the stated SELV limits.

## Electrical design

### Measurement model

GPIO2 supplies a duty-cycled 3.3 V test stimulus through a 1 Mohm reference resistor. ADS1115 measures the divider node. The firmware obtains an estimated resistance:

```text
R_unknown = R_reference × V_node / (V_supply − V_node)
```

The test drive is disabled between samples. The 100 kohm input resistors and enclosure guarding reduce accidental input stress but are not a safety barrier. Use shielded, keyed terminals and a nonconductive enclosure.

### Pin assignment

| ESP32-C6 pin | Net | Function | Constraint |
|---:|---|---|---|
| GPIO0 | MAT_SENSE | optional direct ADC calibration input | no external drive above 3.3 V |
| GPIO1 | STRAP_SENSE | optional direct ADC calibration input | no external drive above 3.3 V |
| GPIO2 | TEST_DRIVE | gated 3.3 V divider excitation | starts low |
| GPIO3 | BUZZER_EN | transistor/buzzer enable | starts low |
| GPIO4 | STATUS_LED | status LED | 1 kohm series resistor |
| GPIO5 | BUTTON | test/acknowledge input | pull-up, active low |
| GPIO6 | I2C_SDA | SHT45 + ADS1115 data | 4.7 kohm pull-up |
| GPIO7 | I2C_SCL | SHT45 + ADS1115 clock | 4.7 kohm pull-up |
| EN | EN | module reset | 10 kohm pull-up; 1 uF reset capacitor |
| GPIO9 | BOOT | programming strap | do not attach bench terminals |

I2C addresses: SHT45 is `0x44`; ADS1115 ADDR to GND is `0x48`. Each IC receives a 100 nF local decoupler; the regulator input and output receive 10 uF bulk capacitors.

### Power architecture and budget

| Mode | Estimate | Notes |
|---|---:|---|
| Deep idle | 0.2 mA | SoC sleep estimate, no radio |
| Sample/measurement | 25 mA | ADC, sensor, LED off |
| Wi-Fi burst (optional future feature) | 180 mA peak | not enabled by reference firmware |
| USB input limit | 500 mA | F1 PTC hold rating |

USB-C uses two 5.1 kohm Rd resistors (CC1/CC2), a 0.5 A PTC, and a 5 V TVS. It is USB-powered only; there is no battery, charger, or power-path ambiguity. Keep the ESP32 antenna region free of copper and large metalwork.

## Firmware

`firmware/` is portable C11 for the measurement/policy core. It separates hardware acquisition from policy evaluation so the math is testable without an ESP-IDF installation. `sdkconfig.defaults` identifies the later ESP-IDF target as `esp32c6`; the environment used for this release did not contain ESP-IDF, so target compilation was not claimed.

```text
firmware/
├── include/esd_core.h       policy and divider interface
├── src/esd_core.c           validated pure logic
├── src/main.c               host demonstration
├── tests/test_esd_core.c    boundary/fault tests
├── CMakeLists.txt
└── sdkconfig.defaults
```

Build and test the host core:

```text
cmake -S firmware -B firmware/build
cmake --build firmware/build
ctest --test-dir firmware/build --output-on-failure
./firmware/build/static_sentinel_demo
```

See [API.md](docs/API.md) for status semantics. A production ESP-IDF port must use the same pin map and must initialize GPIO2/3 low before enabling any peripheral.

## Schematic, BOM, and assembly

- [Reference KiCad schematic](schematic/static-sentinel.kicad_sch)
- [KiCad project settings](schematic/static-sentinel.kicad_pro)
- [BOM](hardware/BOM.csv)
- [Assembly and bring-up](docs/ASSEMBLY.md)
- [Serial/API reference](docs/API.md)
- [Executed validation record](docs/VALIDATION.md)
- [Calibration helper](scripts/check_calibration.py)

The schematic is a native KiCad 7 S-expression reference sheet containing the electrical nets and constraints. There is no PCB layout or fabrication release. Open it in KiCad 7+, replace text-only references with locally resolved symbols/footprints as needed, annotate it, and run ERC before fabrication.

## Safety and limitations

- Use only against isolated ESD mats and wrist straps. Never attach the terminals to mains, powered products, charged capacitors, ionizers, or test equipment outputs.
- The stated resistance bands are example policy defaults, not a substitute for the manufacturer’s ESD-control procedure.
- Relative humidity is context, not proof of static control.
- Verify cable, connector, guard layout, leakage, calibration, and ESD robustness on real hardware. No accuracy, compliance, RF, or physical-validation claim is made here.

## License and credit

MIT. Designed and documented by [jayis1](https://github.com/jayis1).
