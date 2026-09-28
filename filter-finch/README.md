# Filter Finch — Air-filter loading and airflow-context monitor

Filter Finch is an unbuilt, USB-powered reference design for monitoring the pressure drop of a replaceable HVAC filter without confusing a fan-speed change for a dirty filter. One differential-pressure sensor spans the filter; a second reads a small pitot probe as an airflow proxy. An ESP32-C6 normalizes filter drop to a commissioned clean-filter baseline, logs the evidence locally, and reports `OK`, `WARN_LOADING`, `REPLACE`, or an installation fault.

## Status and evidence

| Item | Status |
|---|---|
| Architecture, prototype wiring, BOM, assembly procedure | Complete reference design |
| Portable C normalization/policy core | Host-built and unit-tested |
| Deterministic 120-record simulation | Executed with self-test |
| ESP-IDF sensor/UI/storage adapter | Not implemented |
| Schematic, PCB, ERC/DRC, EMC/listing review | Not supplied / not performed |
| Physical prototype, pressure calibration, duct trials | Not performed |

This is not an indoor-air-quality monitor, a life-safety device, or proof that air is safe. It cannot identify smoke, pathogens, toxic gas, mold, or a ruptured filter. Alert ratios and costs are design starting points, not verified performance specifications.

## Why it is distinct

The collection has room/duct leak mapping, refrigeration diagnostics, particle counting, and appliance sentinels, but no design that measures a filter's own resistance while compensating for changing flow. A single pressure switch can alarm whenever a variable-speed blower ramps up. Filter Finch instead records both quantities and compares a flow-normalized filter drop against a baseline tied to the installed filter and air handler.

The target is a maintainable prototype for workshops, small air handlers, dust collectors, and test rigs. It does not replace a building-management system or the filter/air-handler manufacturer's specified final resistance.

## Architecture

```text
upstream static tap ----+
                        |  SDP810 #1 (0..500 Pa) -- TCA9548A ch0 --+
downstream static tap --+                                         |
                                                                  | I2C
pitot impact ----------+                                          +--> ESP32-C6 DevKit
                       |  SDP810 #2 (0..500 Pa) -- TCA9548A ch1 --+       |-- OLED
pitot static ----------+                                                  |-- microSD CSV
                                                                           `-- button / LED
SHT45 temperature/RH ------------------------ TCA9548A ch2 ----------------+

listed USB 5 V supply --> DevKit regulator --> 3V3 sensors and logic
```

Both SDP810 devices use fixed I2C address `0x25`, hence separate multiplexer channels. The first sensor's `+` port goes upstream and `-` port downstream. The pitot impact port goes to the second sensor's `+` port and its static port to `-`. Use the actual sensor transfer function and CRC checks in the target adapter; a raw I2C response must never be treated as a valid pressure sample without them.

### Pin/bus contract

| ESP32-C6 signal | Prototype connection | Startup requirement |
|---|---|---|
| GPIO6 / GPIO7 | I2C SDA / SCL to TCA9548A | inputs/open drain before bus start |
| mux channel 0 | filter SDP810 at `0x25` | verify product ID and CRC |
| mux channel 1 | pitot SDP810 at `0x25` | verify product ID and CRC |
| mux channel 2 | SHT45 `0x44`, OLED `0x3C` | environmental data are context only |
| GPIO4 / 5 / 2 / 3 | SD SCK / MOSI / MISO / CS | CS high before SPI setup |
| GPIO10 | calibration button, active low | internal pull-up |
| GPIO11 | status LED through 1 kOhm | start low |

The first build uses breakouts and short wiring so the pressure plumbing and algorithm can be validated before designing a PCB. See [assembly and bring-up](docs/ASSEMBLY.md).

## Measurement model

At commissioning, install a known-clean filter and collect stable pairs of filter pressure drop `ΔP_filter` and pitot dynamic pressure `ΔP_flow`. For this reference algorithm:

```text
normalized_drop = ΔP_filter × reference_flow_pressure / ΔP_flow
loading_ratio   = normalized_drop / clean_normalized_drop
```

Pitot dynamic pressure is proportional to air velocity squared. The formula therefore assumes the filter pressure drop is approximately proportional to flow squared over the commissioned operating region. Real filters can follow a different exponent and duct velocity profiles move with dampers and fan speed. The default policy is deliberately modest:

| Condition | Default result |
|---|---|
| pitot pressure <= 0.8 Pa | `FAN_OFF` (no filter judgment) |
| pitot pressure 0.8–4 Pa | `LOW_FLOW` (insufficient evidence) |
| ratio < 1.8 | `OK` |
| ratio 1.8–2.4 | `WARN_LOADING` |
| ratio >= 2.4 | `REPLACE` |
| pressure out of operating range, reversed beyond tolerance, filter drop <0.5 Pa while flow is valid, CRC/timeout failure | fault, no loading claim |

`WARN_LOADING` and `REPLACE` require three consecutive eligible samples in the portable core. Production firmware should also reject fan transitions, retain calibration provenance, and use the manufacturer's absolute final-pressure limit as an independent ceiling.

## Calibration workflow

1. Install the correct new filter in its normal orientation and verify that the duct, dampers, and fan are serviceable.
2. Zero-check both sensors with both ports at the same pressure.
3. Run each normal fan speed long enough to reach steady operation.
4. Collect at least five valid samples above 4 Pa dynamic pressure. The supplied core takes the median of normalized values and rejects low-flow records.
5. Store filter model, installation date, fan mode, clean baseline, firmware version, and raw samples together.
6. Compare both pressure channels with a calibrated low-pressure manometer. Do not deploy thresholds until the observed curves agree with the air-handler and filter documentation.

A calibration is configuration-specific. Changing the filter type, pitot position, fan, pulley, duct, or damper logic requires recommissioning.

## Firmware and simulation

`firmware/` is portable C11 rather than a claimed complete ESP32 image. It contains the testable policy boundary while leaving I2C, display, SD, watchdog, Wi-Fi/BLE, and provisioning to a future ESP-IDF adapter.

```text
firmware/
├── filter_core.h/.c       normalization, calibration, faults, debounce
├── main.c                 host demonstration
├── test_filter_core.c     deterministic boundary tests
└── CMakeLists.txt
scripts/
└── simulate_filter.py     synthetic operating-sequence generator/self-test
```

Build and run:

```text
cmake -S filter-finch/firmware -B filter-finch/firmware/build
cmake --build filter-finch/firmware/build
ctest --test-dir filter-finch/firmware/build --output-on-failure
./filter-finch/firmware/build/filter_finch_demo
python3 -m py_compile filter-finch/scripts/simulate_filter.py
python3 filter-finch/scripts/simulate_filter.py --self-test --output /tmp/filter-finch-simulation.csv
```

The simulation is an algorithm stimulus generator, not computational fluid dynamics and not validation of an SDP810. It creates fan-off, clean-filter, progressively loaded, and reversed-tube records. See the [validation record](docs/VALIDATION.md) for covered and missing evidence.

### Suggested target task split

- 10 Hz: select each mux channel, read SDP810 words, verify CRC, and update filtered pressures.
- 1 Hz: read SHT45, evaluate policy, update alarm debounce, display, and append CSV.
- On calibration request: require a five-minute stable window, store baseline plus raw evidence atomically, and preserve the previous baseline if power fails.
- On any sensor timeout, impossible pressure, full storage, or invalid baseline: show a fault rather than reusing stale data silently.
- Networking, if added, should default off and must not be necessary for local logging or alarms.

## BOM and build scope

The [BOM](hardware/BOM.csv) gives an indicative one-off retail total of about US$158 before shipping, tax, labor, calibration gear, and tools. The pressure sensors dominate cost. Substitution is possible only after checking pressure range, accuracy, media compatibility, response, CRC behavior, supply voltage, and I2C addressing.

The reference prototype has no custom PCB or mains interface. It uses a listed external USB supply, development boards, tubing, two duct taps, and one pitot probe. A production design would require a reviewed schematic/PCB, protected connectors, watchdog and brownout behavior, ESD/EMC work, enclosure/material review, and installation approval.

## Safety and limitations

- De-energize and lock out the air handler before drilling, opening, or servicing ductwork. Verify hidden wiring, coils, refrigerant lines, rotating parts, and dampers first.
- Keep this device out of mains compartments. Never connect an ESP32 or sensor input to thermostat, fan, motor, or line-voltage wiring.
- Do not drill or modify fire/smoke-rated ductwork without the responsible authority's approval. Tubes, grommets, and enclosure materials may be subject to local code.
- Do not use in explosive, flammable, corrosive, condensing, sterile, medical, or life-support air systems. SDP810 media compatibility and pressure limits must be checked against the exact datasheet.
- Kinked, swapped, wet, clogged, or disconnected tubes can create convincing but wrong values. Periodic visual inspection and an independent pressure check are required.
- A representative pitot point is not guaranteed. Duct traverse, flow straightening, or per-speed calibration may be needed.
- The algorithm estimates relative loading; it does not measure filtration efficiency or particle breakthrough. Replace damaged, wet, contaminated, or time-expired filters regardless of the displayed ratio.
- Temperature and humidity are logged only as context. No correction is claimed until characterized on hardware.

## Repository contents

```text
filter-finch/
├── README.md
├── docs/ASSEMBLY.md
├── docs/VALIDATION.md
├── firmware/              portable core, demo, and tests
├── hardware/BOM.csv
└── scripts/simulate_filter.py
```

MIT. Designed and documented by [jayis1](https://github.com/jayis1).