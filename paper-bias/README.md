# Paper Bias

A low-cost bench aid for identifying paper grain direction in small sheets. A fixed-length cantilever bends a paper strip; the RP2040 samples an inexpensive angle sensor and reports relative bend stiffness in two perpendicular orientations. This provides a repeatable comparison for bookbinding, printmaking, and conservation practice—not an absolute material test.

## Novel capability

The workflow compares the same specimen rotated 90 degrees in one fixture and reports a normalized directional stiffness ratio. It differs from the repository's tension, thickness, and optical paper tools: it tests bending anisotropy rather than tensile strength, thickness, or image density.

## Block diagram

```text
USB 5 V -> Raspberry Pi Pico (RP2040)
                         ├─ GP26/ADC0 <- 10 kΩ pot wiper (hinged beam angle)
                         ├─ GP2 <- fixture orientation switch A
                         ├─ GP3 <- fixture orientation switch B
                         └─ USB CDC -> host CSV
```

The sensing fixture is a simple pivoting lever with a repeatable 100 mm paper overhang and a light spring; a potentiometer measures lever angle. This is a relative comparison fixture, not calibrated force instrumentation.

## Pin assignments

| Pico pin | GPIO | Function |
|---|---:|---|
| Pin 31 | GP26 / ADC0 | Potentiometer wiper; 0–3.3 V only |
| Pin 4 | GP2 | orientation A switch, active low |
| Pin 5 | GP3 | orientation B switch, active low |
| Pin 36 | 3V3(OUT) | potentiometer high end |
| Pin 38 | GND | potentiometer low end and switch return |
| USB | — | 5 V power and USB CDC data |

Use a 10 kΩ potentiometer across 3V3/GND. Keep the ADC input below 3.3 V. Orientation switches should be mechanically interlocked so both cannot be active together. Pico module handles USB, clock, flash, and regulation; no bare-chip design is claimed.

## Power architecture

USB 5 V input to Pico, onboard regulator to 3.3 V. Estimated draw 25–60 mA depending on USB activity; no battery, charger, RF, or actuator. Current estimate only, not measured.

## BOM

See [hardware/BOM.csv](hardware/BOM.csv). Components are common reference parts; verify exact footprints before carrier-board layout.

## Firmware and build

`firmware/src/paper_bias.c` contains the portable comparison core. It accepts stable ADC readings and calculates average deflection and a directional ratio. It does not access hardware. The host CMake build checks the logic core; a Pico SDK GPIO/ADC/USB CDC adapter is required for a complete instrument and is not included.

```sh
cmake -S firmware -B /tmp/paper-bias-build
cmake --build /tmp/paper-bias-build
```

CSV integration format: `orientation,adc_mean,samples`; report A/B ratio only when both means exceed the configured baseline deadband. Firmware core deliberately does not claim a calibrated modulus or bending force.

## Schematic/connectivity

See [schematic/netlist.md](schematic/netlist.md). It is an explicit connectivity reference, not a KiCad schematic or PCB. Connect the pot across 3V3 and GND; wiper to GP26. Each switch connects its GPIO to GND and uses the RP2040 internal pull-up in the eventual target adapter.

## Assembly and bring-up

See [docs/ASSEMBLY.md](docs/ASSEMBLY.md). Fixture dimensions and spring force must remain fixed between orientations. Insert a paper strip to the 100 mm stop, record multiple readings in orientation A, rotate the specimen 90 degrees without changing the fixture, then record orientation B. Repeat at least three times and compare medians. Use blank measurements to establish the zero angle.

## API

See [docs/API.md](docs/API.md). The future USB adapter should emit CSV records and refuse invalid orientation states. No USB command parser is included in the portable core.

## Limitations

Reference design only: not fabricated, calibrated, target-built, or physically tested. Fixture friction, humidity, paper curl, thickness, and spring preload affect results. Intended for relative classroom/workshop comparisons, not conservation certification or material acceptance decisions. No safety-critical use.

## License

MIT; authored and maintained by jayis1.
