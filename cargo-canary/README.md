# Cargo Canary — Cellular Cold-Chain, Shock, and Tamper Recorder

Cargo Canary is an unbuilt reference design for a reusable shipment tag that makes handoffs in a cold chain auditable without requiring a nearby phone. Its nRF52840 SoC samples cargo temperature, humidity, door/light exposure, motion shock, and GNSS position; a Quectel BG95-M3 modem uploads compact signed event records through LTE-M or NB-IoT. An e-paper label shows a legible local verdict even with the radio off.

## Status and safe use

| Evidence | Status |
|---|---|
| Architecture, pin map, BOM, KiCad source, and portable C event core | Complete reference design |
| Portable C event core and Python decoder | Host-built and tested in this repository |
| Nordic SDK, modem certification, antenna tuning, KiCad ERC/DRC, thermal calibration, and field trials | Not performed |

This design is not a calibrated thermometer, a legal chain-of-custody system, an aviation tracker, or a safety certification. A shipment must not be accepted or rejected solely from this reference design. Validate sensor placement, cellular coverage, privacy policy, battery transport rules, and the operator's quality procedure before deployment.

## Why it is distinct

The collection includes equipment sentinels, cold appliance diagnostics, and environmental probes, but not a battery shipping tag that preserves shipment evidence across disconnected handoffs. Cargo Canary treats cold excursions, impacts, unsealed doors, and route gaps as separate timestamped facts. It is useful for meal-kit, laboratory reagent, produce, and high-value spare-part pilots where a local display and a later cellular upload are more useful than continuous cloud dependence.

Target PCB: 70 x 43 mm, four layers, with a printed or molded IP54 enclosure and a replaceable 3.6 V Li-SOCl2 primary cell. Estimated electronic BOM: US$48.62 excluding enclosure, assembly, SIM/eSIM, cellular service, certification, and fixtures.

## Block diagram

```text
ER34615 3.6 V cell -> protected load switch -> TPS62745 3V3 -> nRF52840-QIAA-R
                                  |                              |-- I2C: SHT45 (0x44), LIS2DW12 (0x19), OPT3001 (0x44*)
                                  |                              |-- SPI: W25Q128 flash, IL0373 e-paper controller
                                  |                              |-- GPIO: reed tamper, wake button, status LED
                                  |                              |-- UART: BG95-M3 LTE-M/NB-IoT modem
                                  |                              `-- BLE 5 local provisioning/readout
                                  `-> AP9101C undervoltage protection

nRF52840 -> load switch -> BG95-M3 -> LTE-M/NB-IoT antenna
BG95-M3 GNSS -> active patch antenna; 1PPS -> P0.15
```

*The SHT45 and OPT3001 share address 0x44, so they are placed behind TCA9548A I2C switch channels 0 and 1. The LIS2DW12 uses 0x19. I2C pull-ups are 4.7 kOhm on the controller side and each enabled downstream bus.

## Pin and bus contract

| nRF52840 pin | Net | Connection | Notes |
|---:|---|---|---|
| P0.26 / P0.27 | I2C_SDA / I2C_SCL | TCA9548A, 400 kHz | 4.7 kOhm pull-ups to 3V3 |
| P0.13 / P0.14 | SPI_SCK / SPI_MOSI | W25Q128 + e-paper | shared bus; separate chip selects |
| P0.15 | SPI_MISO / GNSS_PPS | W25Q128 MISO / BG95 1PPS | firmware multiplexes after flash read |
| P0.16 | FLASH_CS_N | W25Q128 | 16 MiB event store |
| P0.17 | EPD_CS_N | IL0373 display | 2.13 inch e-paper |
| P0.18 | EPD_BUSY | IL0373 busy | wake-capable input |
| P0.19 | EPD_DC | IL0373 data/command | output |
| P0.20 | MODEM_PWRKEY | BG95 PWRKEY | open-drain transistor, 650 ms pulse |
| P0.21 / P0.22 | MODEM_TX / MODEM_RX | BG95 UART | 115200 baud, level-compatible 1.8 V translator |
| P0.23 | MODEM_STATUS | BG95 STATUS | input through translator |
| P0.24 | MODEM_EN | TPS22919 modem load switch | only power modem for scheduled/event upload |
| P0.11 | TAMPER_N | normally-closed reed loop | internal pull-up; open means enclosure opened |
| P0.12 | WAKE_N | button to ground | internal pull-up |
| P0.25 | INT_MOTION | LIS2DW12 INT1 | shock/motion wake |
| P0.31 / AIN7 | VBAT_SENSE | 1 Mohm / 330 kohm divider | switched divider; max < VDD |
| P0.06 | STATUS_LED | green LED through 1 kOhm | active high |
| P0.18 / P0.19 | SWDIO / SWCLK | programming pads | fixture only; do not expose externally |

## Event model and failure behavior

Every event carries a monotonically increasing sequence number, UTC estimate, elapsed monotonic seconds, current temperature in centi-degrees C, shock peak in mg, battery mV, and flags. Events remain in NOR flash before and after an upload; a server acknowledgement is recorded separately. This prevents a radio failure from erasing evidence.

- Temperature is sampled every 5 minutes; a cold-excursion event opens after 10 consecutive out-of-range samples and closes only after 3 in-range samples.
- LIS2DW12 FIFO wake detects a configurable 8 g, 10 ms shock. The firmware stores peak magnitude and a 2 second pre/post summary, not raw continuous location data.
- Light plus a broken reed loop produces a tamper event. A light-only event is marked `SUSPECT_OPEN`, not proof of opening.
- If GNSS has no fix in 90 seconds, the tag stores `LOCATION_UNAVAILABLE`; it does not invent a position.
- If modem upload fails, records remain queued with exponential retry capped at two radio sessions per hour. Low battery disables the modem and keeps local logging/display alive.

## Power architecture and budget

```text
ER34615 3.6 V Li-SOCl2 -> F1 0.75 A PTC -> D1 TVS -> U1 TPS62745 3.3 V
                                                -> U2 TPS22919 switched 3.8 V modem rail
                                                -> U3 AP9101C battery protection monitor
```

| Mode | Estimated current | Design intent |
|---|---:|---|
| RTC + sensors between samples | 13 uA | nRF System ON idle, sensor duty cycled |
| 5-minute measurement + flash append | 2.1 mA for 250 ms | normal logger work |
| E-paper refresh | 18 mA for 1.5 s | only at handoff or state change |
| GNSS acquisition | 28 mA typical | bounded 90 s attempt |
| LTE-M transmit burst | 450 mA peak | local 470 uF low-ESR reservoir required |

A 19 Ah ER34615 cell is a shipping classification decision, not an automatic approval. At one cellular upload every 6 hours and one GNSS fix per day, the conceptual budget supports approximately 8–14 months; modem coverage, temperature, cell pulse capability, and display use dominate the real result. Use a hybrid-layer capacitor or a certified high-pulse cell if field profiling shows excessive brownouts.

## Schematic and layout constraints

- [KiCad project](schematic/cargo-canary.kicad_pro), [schematic source](schematic/cargo-canary.kicad_sch), and [PCB source](schematic/cargo-canary.kicad_pcb)
- [Bill of materials](hardware/BOM.csv)
- [Assembly and bring-up guide](docs/ASSEMBLY.md)
- [Protocol/API reference](docs/API.md)
- [Offline event decoder](scripts/decode_events.py)

Place BG95, its 470 uF reservoir, and the LTE antenna at the board edge with the vendor antenna keep-out. Keep the nRF antenna and LTE antenna separated per their datasheets. Do not route SPI or I2C under either antenna. Use a four-layer stack with continuous ground plane, 50 ohm controlled-impedance RF feeds, an isolated switching return, and a guarded battery-sense divider. Validate all footprints, cellular RF layout, antenna matching, creepage, and ERC/DRC in the exact KiCad library release before fabrication.

## Firmware

```text
firmware/
├── cargo_canary.h / cargo_canary.c  event encoding, excursion state machine, CRC-16
├── main.c                           host demonstration adapter
├── test_cargo_canary.c              deterministic portable-core tests
├── CMakeLists.txt                   C11 build and CTest configuration
└── sdkconfig.defaults               target integration contract
```

Run the checks:

```text
cmake -S firmware -B firmware/build
cmake --build firmware/build
ctest --test-dir firmware/build --output-on-failure
python3 -m py_compile scripts/decode_events.py
python3 scripts/decode_events.py --self-test
```

The target adapter must use Nordic-approved BLE and power-management APIs, configure the watchdog after persistent storage is ready, use modem vendor power sequencing, validate server certificates, rate-limit radio retries, and never mark a queued record delivered until a server receipt includes its sequence and CRC.

## Directory structure

```text
cargo-canary/
├── README.md
├── schematic/    KiCad project, schematic, PCB source
├── firmware/     portable C core, test, CMake configuration
├── hardware/     BOM
├── docs/         assembly and API references
└── scripts/      offline decoder
```

## Credit

MIT. Designed and documented by [jayis1](https://github.com/jayis1).