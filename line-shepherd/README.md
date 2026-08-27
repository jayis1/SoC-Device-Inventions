# Line Shepherd — Pocket TDR Cable Fault Locator & Pair Mapper

> A field-buildable handheld cable diagnostic node that finds opens, shorts, water ingress, bridge taps, poor splices, and split-pair wiring in Ethernet, alarm, irrigation, gate, and utility control cables.

## What It Is

**Line Shepherd** is a practical open hardware cable-inspection instrument built around a **STM32G474RET6** measurement MCU and an **ESP32-C3-MINI-1** wireless companion. It combines fast-pulse **time-domain reflectometry (TDR)**, pair selection, continuity measurement, and low-frequency cable fingerprinting into a single device that can be carried by installers, maintenance teams, AV technicians, farm operators, and facility electricians.

Instead of guessing where a buried cable failed or walking back and forth with a multimeter, Line Shepherd launches a controlled fast edge into the selected pair, timestamps the first major reflection, inspects reflection polarity and ringing, and fuses that with resistance/capacitance balance data to identify likely faults and estimate where they are.

## Why This Device Matters

A huge amount of downtime comes from hidden cable faults:

- irrigation valve runs broken by shovels or rodents
- gate and access-control loops corroded underground
- Ethernet drops with bad terminations or split pairs
- alarm and sensor cables with moisture ingress
- long fence or field wiring with intermittent shorts
- bridge taps and undocumented splices in retrofits

Commercial TDR instruments exist, but many are expensive, closed, or specialized. Line Shepherd is designed to be **buildable, understandable, and adaptable**.

## Main SoC Choice

### STM32G474RET6

The STM32G474RET6 is the measurement heart because it offers:

- 170 MHz Cortex-M4F with DSP instructions
- fast timers and input capture for sub-10 ns event timing after comparator conditioning
- multiple ADCs for resistance, battery, and analog envelope measurements
- hardware op-amps and comparators useful for mixed-signal instrumentation
- enough GPIO for muxing, display, SD, and the analog front end

### ESP32-C3-MINI-1

The ESP32-C3 companion provides:

- BLE for phone-based cable reports
- Wi-Fi for firmware updates and job synchronization
- USB/UART service access
- a simple separation between precision timing and connected UI tasks

## Functional Overview

1. The user connects RJ45, terminal block, or banana clip leads to the cable under test.
2. High-voltage lines are blocked by resettable protection and TVS networks before the instrument connects.
3. An **ADG1608 analog mux** selects the active pair or conductor set.
4. The STM32 fires a controlled launch pulse through a **74LVC2G04 edge driver** and resistive launch network.
5. Reflections from impedance changes travel back to a **THS3201 + ADCMP600** receive chain.
6. Timer capture hardware timestamps threshold crossings while the ADC samples a low-rate envelope and DC characterization channels.
7. Firmware estimates:
   - distance to first major fault
   - open vs short polarity
   - likely splice / bridge-tap / water-ingress signature
   - loop resistance
   - capacitance per meter estimate
   - pair balance and split-pair suspicion
8. Results appear on the OLED and can be logged locally or exported over BLE/Wi-Fi.

## Typical Fault Classes

- **OPEN_END** — cable reaches an open termination or severed conductor
- **SHORT_END** — direct short or crushed insulation at the measured distance
- **HIGH_RESISTANCE_SPLICE** — partially reflective splice, corroded crimp, or weak IDC punchdown
- **WATER_INGRESS** — broadened negative-going reflection and elevated capacitance / imbalance
- **BRIDGE_TAP** — double reflection caused by an unterminated branch
- **SPLIT_PAIR** — continuity exists, but capacitance balance and crosstalk proxy indicate pair mis-termination
- **HEALTHY_LINE** — matched endpoint or expected termination with acceptable balance
- **OVERVOLTAGE_LOCKOUT** — foreign voltage detected; pulse launch inhibited

## Block Diagram

```text
┌────────────────────────────── Line Shepherd ──────────────────────────────┐
│                                                                           │
│ RJ45 jack ─┐                                                              │
│ Terminal   ├─► TVS + PTC + surge resistors ─► ADG1608 pair mux ─┐         │
│ Banana in ─┘                                                     │         │
│                                                                   ▼         │
│                     74LVC2G04 pulse driver ─► launch bridge ─► cable       │
│                                                                   ▲         │
│ cable reflection ─► THS3201 wideband amp ─► ADCMP600 comparator ─┤         │
│ cable DC path ────► current source + INA186 + ADC ────────────────┤         │
│ cable C path ─────► charge timing network + ADC ───────────────────┤         │
│                                                                    ▼         │
│                          STM32G474RET6 measurement MCU                      │
│                          - pulse timing                                      │
│                          - classification                                    │
│                          - logging                                           │
│                          - OLED UI                                           │
│                               │                                              │
│                               ├── SPI ─► microSD                            │
│                               ├── I2C ─► OLED + RTC + fuel gauge            │
│                               └── UART ─► ESP32-C3-MINI-1 BLE/Wi-Fi bridge  │
│                                                                           │
│ USB-C / Li-ion ─► charger ─► 3V3 digital ─► 3V3 analog LDO / 5V boost      │
└───────────────────────────────────────────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | STM32G474RET6 | Main measurement MCU, timing engine, classification |
| U2 | ESP32-C3-MINI-1 | BLE/Wi-Fi companion and service bridge |
| U3 | ADG1608BRUZ | 8:1 analog mux for pair/conductor selection |
| U4 | THS3201DBVT | Wideband receive amplifier for reflected pulse sensing |
| U5 | ADCMP600BRJZ | High-speed comparator for reflection threshold timing |
| U6 | INA186A2IDDFR | Differential amplifier for loop resistance measurement |
| U7 | MAX17260SEWL | Li-ion fuel gauge |
| U8 | MCP73831T-2ATI/OT | 1-cell Li-ion charger |
| U9 | TPS61023DRLR | 5 V boost for pulse-launch headroom |
| U10 | TPS63031DSKR | Main 3.3 V buck-boost rail |
| U11 | TLV75533PDBVR | Quiet analog 3.3 V rail |
| U12 | DS3231SN | RTC for timestamped logs |
| DS1 | SH1106 128x64 OLED | Local display |
| J1 | RJ45 magjack-less connector | Twisted-pair interface |
| J2 | 6-position terminal block | Alarm/irrigation/fence wiring |
| J3 | Banana jack pair | General test lead input |
| J4 | microSD socket | Logging |
| D1-D8 | SMF05C / SMBJ arrays | ESD and surge protection |
| F1-F4 | MF-R050 PTC | Foreign-voltage and fault current limiting |
| Q1-Q4 | 74LVC2G04 / MOSFET launch path | Fast pulse generation |

### Measurement Topology

#### 1. Pulse Launch Path

The TDR path is deliberately simple and serviceable:

- **TPS61023** generates a 5 V launch rail.
- A **74LVC2G04** dual inverter is paralleled for a fast edge.
- A 49.9 Ω source resistor and switchable 25 Ω/75 Ω damping network approximate cable-family source impedance.
- Launch pulse width is programmable from **8 ns to 200 ns** using STM32 timer gating and analog switch control.

This enables short, sharp reflections on short twisted-pair runs while allowing wider pulses for long irrigation or fence loops.

#### 2. Reflection Receive Path

The reflection node feeds:

- **THS3201** wideband gain stage at ~6 dB
- clamp diodes to the analog rail
- **ADCMP600** comparator with selectable threshold DAC reference from the STM32
- STM32 timer input capture for nanosecond-scale round-trip timing

An auxiliary RC detector also provides a lower-bandwidth envelope into an STM32 ADC channel for signature broadening and ringing analysis.

#### 3. Resistance / Continuity Path

For DC diagnostics, the STM32 enables a precision current source:

- 1 mA / 10 mA programmable source through cable loop
- **INA186** senses differential drop
- STM32 ADC computes loop resistance

This distinguishes a clean short from a corroded splice or partially broken conductor.

#### 4. Capacitance / Balance Path

Each conductor can be charged through a precision resistor while the STM32 measures charge time and discharge asymmetry. The derived capacitance and conductor-to-conductor imbalance help detect:

- water ingress
- crushed dielectric
- split-pair Ethernet terminations
- unexpected bridge taps

### Protection Network

Cable instruments encounter unsafe lines often, so Line Shepherd checks before launch:

- PTC resettable fuses on each input branch
- bidirectional TVS arrays to chassis and signal ground
- 1 MΩ foreign-voltage sensing divider into STM32 ADC
- firmware interlock if sensed common-mode exceeds **±18 V**
- optional gas-discharge tube pads for outdoor deployments

### Buses and Interconnects

- **SPI1**: microSD
- **I2C1**: OLED, DS3231, MAX17260
- **UART3**: STM32 ↔ ESP32-C3 bridge
- **ADC inputs**: envelope, loop resistance, battery, foreign voltage, capacitance timing node
- **Timer capture**: comparator output, pulse width monitor

## STM32G474 Pin Assignment

| Pin | Function |
|-----|----------|
| PA0 | REFLECT_ENV_ADC |
| PA1 | LOOP_RES_ADC |
| PA2 | FOREIGN_VOLT_ADC |
| PA3 | CAP_TIMING_ADC |
| PA4 | THRESH_DAC_OUT |
| PA5 | SPI1_SCK (microSD) |
| PA6 | SPI1_MISO |
| PA7 | SPI1_MOSI |
| PB0 | MUX_A0 |
| PB1 | MUX_A1 |
| PB2 | MUX_A2 |
| PB10 | I2C1_SCL |
| PB11 | I2C1_SDA |
| PB12 | SD_CS |
| PB13 | OLED_RESET |
| PB14 | LAUNCH_WIDTH_SEL0 |
| PB15 | LAUNCH_WIDTH_SEL1 |
| PC6 | COMP_CAPTURE_IN |
| PC7 | PULSE_MONITOR_IN |
| PC8 | BUTTON_MODE |
| PC9 | BUTTON_FIRE |
| PC10 | BUTTON_SAVE |
| PC11 | BUZZER_EN |
| PC12 | STATUS_LED |
| PD2 | UART3_RX (from ESP32-C3) |
| PC10/PD8 alt | UART3_TX (to ESP32-C3) |
| PA8 | CURRENT_SRC_EN |
| PA9 | CURRENT_RANGE_SEL |
| PA10 | BOOST_EN |
| PA15 | SENSOR_POWER_EN |

> Final PCB routing must validate alternate-function conflicts against the exact STM32 package and boot configuration.

## ESP32-C3 Pin Assignment

| Pin | Function |
|-----|----------|
| GPIO0 | Boot/service button |
| GPIO1 | UART_RX from STM32 |
| GPIO2 | UART_TX to STM32 |
| GPIO3 | Wi-Fi status LED |
| GPIO4 | BLE pairing button |
| GPIO5 | SD card activity input (optional) |
| GPIO6 | I2C_SDA optional accessory header |
| GPIO7 | I2C_SCL optional accessory header |
| GPIO8 | USB/service detect |
| GPIO9 | Wake / interrupt from STM32 |

## Power Architecture

### Inputs

- USB-C 5 V for charging and bench power
- 1-cell Li-ion battery, 1800-2600 mAh typical

### Rails

- **VBAT** — raw battery
- **VSYS_3V3** — TPS63031 system rail for digital logic
- **ANA_3V3** — TLV75533 low-noise rail for comparator/amplifier/reference network
- **PULSE_5V** — TPS61023 boost rail for launch pulse driver
- **SENSE_SW** — switched measurement rail for mux and front-end sections

### Estimated Consumption

| Mode | Current |
|------|---------|
| Deep sleep with RTC | ~180 µA |
| Idle UI | ~38 mA |
| Active TDR scan | ~74 mA |
| Wi-Fi sync + SD write peak | ~158 mA |

A 2200 mAh cell yields roughly **18-24 hours** of intermittent field troubleshooting.

## Connector Strategy

- **RJ45** for CAT5e/CAT6, intercom, and low-voltage twisted pair
- **6-way terminal block** for irrigation valves, gate loops, security cable, and fence wire
- **banana jacks** for clip leads and ad-hoc continuity work
- optional **remote ID terminators**: resistor-coded plugs that let the device identify far-end outlets without active electronics

## Expected Performance

| Metric | Target |
|--------|--------|
| Fault distance range | 0.5 m to 400 m depending on cable type |
| Short/open distance accuracy | ±(1 m + 1.5% of reading) after VF calibration |
| Minimum resolvable branch separation | ~3 m on CAT5e |
| Foreign voltage lockout | > ±18 V common-mode |
| Supported cable families | CAT5e/6, alarm, irrigation multi-core, coax-adjacent two-wire, fence wire |

## Firmware Architecture

The included firmware folder is a **host-buildable C simulation** of the embedded decision pipeline. It models field captures and demonstrates classification, distance estimation, and health scoring before a board-specific HAL port.

### Source Layout

```text
firmware/
├── CMakeLists.txt
├── sdkconfig
├── include/
│   ├── line_shepherd.h
│   ├── sensors.h
│   ├── tdr.h
│   ├── classifier.h
│   ├── line_db.h
│   └── ui.h
└── src/
    ├── main.c
    ├── sensors.c
    ├── tdr.c
    ├── classifier.c
    ├── line_db.c
    └── ui.c
```

### Feature Extraction Chain

```text
reflection timing + polarity + ringing + resistance + capacitance + balance
    -> round-trip time estimate
    -> distance from velocity factor
    -> primary reflection polarity
    -> branch / splice / moisture heuristics
    -> balance quality score
    -> health score
    -> final fault class
```

## Build and Run the Firmware Simulation

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/line_shepherd_fw_sim
```

## Example Simulation Output

```text
Asset                 Pair    Fault                   Dist(m)  Health
IRR-VALVE-07          PAIR_B  OPEN_END                 38.7    54.1
WAREHOUSE-DROP12      PAIR_A  SPLIT_PAIR               27.3    61.5
GATE-LOOP-WEST        PAIR_C  WATER_INGRESS           61.4    21.2
OFFICE-BRANCH-03      PAIR_D  BRIDGE_TAP              18.4    42.5
PUMP-HOUSE-SENS1      PAIR_A  HIGH_RESISTANCE_SPLICE  12.1    52.6
CONF-RM-04            PAIR_B  HEALTHY_LINE            30.8    91.5
```

## Calibration Workflow

1. Connect a known-good cable of measured length.
2. Select cable family and enter velocity factor or perform auto-fit against known length.
3. Run open-end and shorted-end fixtures to calibrate threshold crossing delay.
4. Store per-cable-family offset in flash.
5. Verify resistance current-source accuracy with a 10 Ω standard.
6. Verify capacitance timing with 10 nF and 47 nF references.

## Build Notes

- Use a **4-layer PCB** with a continuous ground plane under digital logic and a controlled reflection-launch region.
- Keep the launch bridge, comparator input, and protection network compact.
- Route outdoor connectors with wide creepage, replaceable protection parts, and clear chassis/logic-ground strategy.
- Add a grounded shield wall between the pulse-launch area and the OLED/radio section if enclosure space permits.

## Files in This Folder

- `schematic/` — conceptual KiCad source files
- `firmware/` — C simulation of the fault-detection pipeline
- `hardware/BOM.csv` — major buildable components
- `docs/assembly-guide.md` — assembly and bring-up guidance
- `docs/api-reference.md` — CSV/BLE/Wi-Fi payload structure
- `scripts/` — helper tools for fixture log generation and analysis

## Unified TREE Role

Line Shepherd is primarily an **Observe** and **Act** node:

- **Observe**: captures infrastructure-health telemetry from hidden low-voltage cabling
- **Act**: helps technicians localize and resolve faults faster
- can feed maintenance events into larger building, farm, or industrial monitoring systems
