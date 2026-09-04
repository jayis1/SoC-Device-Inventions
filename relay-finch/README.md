# Relay Finch — Pocket Relay and Solenoid Signature Analyzer

> A handheld maintenance instrument that energizes DC coils, measures armature motion and contact bounce, and decides whether a relay, valve coil, or contactor is healthy before it fails in service.

## What It Is

**Relay Finch** is a buildable test instrument for technicians who need more than a continuity beep. It drives a relay or solenoid coil with a programmable voltage ramp, watches the coil current waveform, listens to the actuation click through a contact piezo sensor, tracks armature travel with a Hall sensor, and times the contact transitions on an isolated sense input.

The result is a real electromechanical signature for every actuation:

- pull-in voltage
- drop-out voltage
- operate time
- release time
- bounce duration
- coil resistance estimate
- pickup and hold current
- armature travel margin
- sluggishness / sticking indicators
- welded-contact and weak-spring detection

## Why This Device Matters

Relays, small contactors, latching valves, and interposing coils fail long before they look burnt. Typical early faults are subtle:

- rising operate time from dirt or gummy grease
- weak springs causing late release
- pitted contacts causing extended bounce
- shorted turns lowering pull-in voltage but increasing heat
- partial armature travel from low supply or coil damage
- welded contacts that stay closed after drive is removed

Relay Finch catches those faults on the bench or in the field with a pocket instrument instead of a scope, bench supply, and improvised wiring.

Useful applications:

- HVAC contactors and defrost relays
- irrigation and industrial valve coils
- vehicle relays and fuel-pump relays
- PLC cabinet maintenance
- appliance repair
- incoming inspection and life-cycle testing in small manufacturing

## Main SoC Choice

### STM32G474RET6 + ESP32-C3-MINI-1

The design splits precision measurement from wireless connectivity:

- **STM32G474RET6** handles waveform capture, timers, ADC triggering, and fast real-time analytics.
- **ESP32-C3-MINI-1** handles BLE, Wi-Fi, phone sync, and OTA updates.

Why this split works:

- STM32G4 gives precise timers, DAC, comparators, op-amp support, and DSP-friendly math.
- ESP32-C3 keeps the UI and cloud path simple without burdening the measurement loop.
- The architecture isolates noisy radio activity from the analog front end.

## Functional Overview

1. The operator clips the DUT coil and contact terminals to the test leads.
2. A protected programmable driver applies either a stepped, ramped, or pulsed excitation from **3 V to 48 V**.
3. A low-side shunt plus **INA240** measures coil current at high slew rates.
4. An isolated contact-sense path detects when the DUT contacts actually change state.
5. A **TMAG5273** Hall sensor watches armature travel using a small temporary magnet on the relay armature or valve plunger.
6. A piezo contact disc bonded to the DUT body captures the click and ringing envelope.
7. The STM32G474 extracts event features and classifies the DUT state.
8. Results appear on the OLED, are logged to microSD, and can be exported by BLE or Wi-Fi.

## Block Diagram

```text
          ┌──────────────────── Relay Finch ────────────────────┐
 DUT coil ─► protected high-side driver ─┐                      │
                                         ├─ shunt ─ INA240 ─┐   │
                                         │                  │   │
 DUT aux/contact ─► isolated contact sense ───────────────┐ │   │
 DUT armature ─► magnet + TMAG5273 Hall travel sensor ────┼─┼──►│ STM32G474RET6
 DUT frame ───► piezo contact microphone + envelope AFE ──┘ │   │ - timing capture
                                                             │   │ - current analytics
 USB-C / Li-ion / 12 V in ─► power tree ─────────────────────┘   │ - fault classifier
                                                                 │ - OLED UI
 microSD ────────────────────────────────────────────────────────►│ - log writer
                                                                 ├──── UART ───► ESP32-C3-MINI-1
 buttons / rotary encoder ──────────────────────────────────────►│              BLE + Wi-Fi sync
 1.3" OLED ─────────────────────────────────────────────────────►│
          └──────────────────────────────────────────────────────┘
```

## Full Schematic Design

### Major ICs and Subsystems

| Ref | Part | Function |
|-----|------|----------|
| U1 | STM32G474RET6 | Main measurement MCU |
| U2 | ESP32-C3-MINI-1 | Wireless bridge, OTA, mobile API |
| U3 | INA240A1IDR | Bidirectional current-sense amplifier for coil shunt |
| U4 | TMAG5273A1QDBVR | I2C Hall sensor for armature travel |
| U5 | ISO7721FDWR | Isolated digital sense for DUT contact state |
| U6 | MCP6002T-I/SN | Piezo envelope amplifier and anti-alias filtering |
| U7 | LM5155QDSSRQ1 | Boost controller for programmable coil drive rail |
| U8 | TPS54560BDDAR | Buck regulator for 5 V system rail |
| U9 | TPS62172DSGR | 3.3 V logic rail |
| U10 | MCP73831T-2ACI/OT | Li-ion charger |
| U11 | MAX17048G+T10 | Fuel gauge |
| U12 | DS3231SN | RTC for timestamped logs |
| U13 | ADS1015IDGST | Auxiliary 12-bit ADC for battery/input monitoring |
| Q1 | IPT015N10N5 | Main low-Rds(on) coil drive MOSFET |
| Q2 | AO3400A | Piezo bias / buzzer / utility switch |
| D1 | SMBJ58A | Input surge clamp for field wiring |
| D2 | STTH2R06 | Fast recirculation path for coil step-down tests |
| DS1 | SH1106 OLED 128x64 | Local display |
| J1 | microSD socket | Result logging |
| J2 | USB-C receptacle | Charging, debug, data |
| J3 | DUT coil output | Kelvin clip lead connector |
| J4 | DUT contact input | Isolated contact sense connector |
| J5 | Hall probe jack | Remote probe or onboard clip sensor |
| BZ1 | Piezo buzzer | Pass/fail alert |

### Measurement Path

#### Coil drive

Relay Finch supports most common **DC coils from 3 V to 48 V** and brief currents up to **2 A**.

- input power may come from USB-C plus Li-ion battery for low-voltage coils
- external 12–24 V barrel input is preferred for larger coils
- **LM5155** boost stage generates an adjustable **COIL_BUS** up to 52 V
- **Q1** PWM-modulates the low side under STM32 timer control
- freewheel path can be switched between:
  - **fast diode clamp** for normal release timing
  - **active decay / resistor dump** for accelerated release testing

#### Current measurement

- **10 mΩ four-terminal shunt** in the low side
- **INA240A1** measures current with high common-mode transient rejection
- STM32 ADC samples the amplified shunt at up to **100 kS/s** during capture windows
- firmware derives:
  - pickup current
  - hold current
  - steady-state current
  - current slope `di/dt`
  - inferred coil resistance and electrical time constant

#### Contact sensing

The DUT contact state is sensed through an isolated input so the device can inspect dry contacts or externally biased contacts safely.

- current-limited sense resistor network
- optically or capacitively isolated logic stage via **ISO7721**
- timer input capture measures the first closure and every bounce edge
- firmware computes bounce duration and chatter count

#### Armature travel sensing

A removable magnet sticker or clip is placed near the moving armature.

- **TMAG5273** samples field strength over I2C
- calibration maps field change to approximate travel in millimeters
- travel trace detects incomplete pull-in, sticky motion, and delayed release

#### Acoustic / vibration click sensing

A piezo disc couples to the relay body.

- passive high-pass behavior rejects slow drift
- **MCP6002** amplifies and rectifies the envelope
- ADC captures the click onset and post-impact ringing
- acoustic timing cross-checks the Hall and contact transitions

## Detailed Interconnects

### STM32G474 Pin Assignment

| STM32 Pin | Function |
|-----------|----------|
| PA0 | coil current ADC input |
| PA1 | piezo envelope ADC input |
| PA2 | battery / input monitor ADC |
| PA3 | COIL_PWM drive |
| PA4 | decay-mode control |
| PA5 | SPI1_SCK to microSD |
| PA6 | SPI1_MISO |
| PA7 | SPI1_MOSI |
| PB0 | DUT contact isolated input capture |
| PB1 | buzzer drive |
| PB6 | I2C1_SCL |
| PB7 | I2C1_SDA |
| PB8 | OLED reset |
| PB9 | OLED D/C or mode GPIO |
| PB10 | UART3_TX to ESP32-C3 |
| PB11 | UART3_RX from ESP32-C3 |
| PB12 | microSD CS |
| PB13 | encoder A |
| PB14 | encoder B |
| PB15 | encoder push |
| PC6 | Hall data-ready interrupt |
| PC7 | start/stop button |
| PC8 | mode button |
| PC9 | charger status |
| PC10 | RTC interrupt |
| PC11 | sensor rail enable |
| PC12 | fault latch input |

### ESP32-C3 Pin Assignment

| ESP32-C3 Pin | Function |
|--------------|----------|
| GPIO0 | boot strap / service pad |
| GPIO1 | UART RX from STM32 |
| GPIO2 | UART TX to STM32 |
| GPIO3 | status LED |
| GPIO4 | wake / sync request |
| GPIO5 | SD card sharing detect |
| GPIO6 | I2C SDA optional service bus |
| GPIO7 | I2C SCL optional service bus |
| GPIO18 | USB/JTAG debug |
| GPIO19 | USB/JTAG debug |

> Final PCB layout must verify STM32 analog routing, ESP32-C3 strapping pins, and the exact isolation barrier creepage for the chosen enclosure and safety target.

## Power Architecture

### Inputs

1. **USB-C 5 V** for charging, firmware update, and light-duty bench work
2. **1-cell Li-ion** for portable use
3. **12–24 V DC barrel input** for higher-power coil testing

### Rails

- **VIN_EXT**: 12–24 V external input
- **VBUS_5V**: USB-C input
- **VBAT**: Li-ion pack
- **SYS_5V**: buck output for display, SD, and support logic
- **3V3_DIG**: STM32, ESP32-C3, Hall, RTC, OLED
- **COIL_BUS**: programmable 3–52 V test rail from boost stage
- **PIEZO_BIAS**: mid-rail analog bias for acoustic envelope front end

### Estimated Power Budget

| Mode | Current |
|------|---------|
| idle, display dim | ~26 mA |
| active test, low-voltage coil | ~90 mA + DUT current |
| Wi-Fi upload burst | ~210 mA peak + DUT current |
| battery charging only | ~35–500 mA depending on charge setting |

## Test Modes

### 1. Pull-in sweep

The device ramps coil voltage upward until motion and contact closure occur.

Outputs:

- pull-in voltage
- pickup current
- operate time from drive start
- travel margin at final drive voltage

### 2. Hold and release sweep

The device energizes the coil, then ramps voltage down until release.

Outputs:

- drop-out voltage
- hold current
- release time
- residual magnetism suspicion

### 3. Bounce capture

The device applies a step input and captures all contact edges in the first 20 ms after closure.

Outputs:

- bounce duration
- chatter count
- first stable close time

### 4. Repeated life spot-check

Runs N repeated actuations and tracks drift in:

- operate time
- bounce
- steady-state current
- acoustic ringing

## Fault States

1. **PASS** — actuation metrics inside preset limits
2. **SLOW_PULL_IN** — operate time too long or travel too shallow
3. **EXCESSIVE_BOUNCE** — bounce duration or chatter count too high
4. **LOW_COIL_RESISTANCE** — possible shorted turns or wrong coil
5. **WEAK_RETURN_SPRING** — release late or drop-out voltage too low
6. **WELDED_CONTACTS** — contacts remain closed after drive removal
7. **NO_ACTUATION** — insufficient travel or no closure detected

## Preset Library

Default presets stored in flash:

| Preset | Nominal Coil | Pull-in Limit | Drop-out Window | Max Bounce |
|--------|--------------|---------------|-----------------|------------|
| automotive_relay_12v | 12 V | ≤ 9.5 V | 1.5–5.0 V | 3.0 ms |
| plc_relay_24v | 24 V | ≤ 19.0 V | 3.0–10.0 V | 4.0 ms |
| valve_coil_24v | 24 V | ≤ 20.5 V | 2.0–8.0 V | n/a |
| contactor_aux_24v | 24 V | ≤ 21.0 V | 3.0–9.0 V | 5.0 ms |

## Firmware Layout

```text
firmware/
├── CMakeLists.txt
├── include/
│   ├── relay_finch.h
│   ├── sensors.h
│   ├── analysis.h
│   └── classifier.h
└── src/
    ├── main.c
    ├── sensors.c
    ├── analysis.c
    └── classifier.c
```

The included host-build demo simulates several relay captures and exercises the analysis pipeline without requiring embedded hardware.

## Helper Scripts

- `scripts/analyze_capture.py` parses CSV captures and prints per-event metrics
- `scripts/demo_capture.csv` is a sample export matching the device API format

## Build and Run the Firmware Demo

```bash
cd firmware
cmake -S . -B build
cmake --build build
./build/relay_finch_demo
```

## Manufacturing Notes

- keep the shunt, INA240, and ADC input return tightly coupled
- isolate the ESP32-C3 antenna edge from the Hall and piezo analog front end
- route the high-current coil loop away from the Hall probe traces
- add TVS and reverse-polarity protection to all field connectors
- use creepage slots if the contact-sense front end will see dirty industrial wiring

## Limitations

- this design targets **DC coils**, not direct mains AC contactor coils
- the isolated contact input is for sensing status, not switching load current
- Hall-based travel sensing needs a small temporary magnet or a target clip
- coil current above 2 A continuous needs a larger MOSFET, shunt, and thermal design

## Repository Contents

```text
relay-finch/
├── README.md
├── schematic/
│   ├── relay-finch.kicad_pro
│   ├── relay-finch.kicad_sch
│   └── relay-finch.kicad_pcb
├── firmware/
│   ├── CMakeLists.txt
│   ├── sdkconfig
│   ├── include/
│   └── src/
├── hardware/
│   └── BOM.csv
├── docs/
│   ├── assembly-guide.md
│   └── api-reference.md
└── scripts/
    ├── analyze_capture.py
    └── demo_capture.csv
```

## Next Build Steps

1. build the analog front end and verify coil current scaling with a dummy resistor load
2. validate the Hall travel clip on a known relay with transparent cover
3. compare bounce timing against a benchtop oscilloscope for calibration
4. characterize release timing under diode, Zener, and active-decay suppression schemes

## License

MIT
