# Selvage Scout

Offline fabric-edge inspection aid for comparing fray resistance between textile samples. The user draws a small swatch edge through a repeatable, low-force comb; a reflective sensor records each released fiber bundle as a pulse train. An RP2040 counts events and reports relative counts per manually entered test length over USB CDC. It does not determine textile grade or replace standardized abrasion/fray testing.

## Novelty
Unlike this repository's force–travel, yarn stretch, twist, and fabric-feed devices, this fixture measures optically detected loose-fiber shedding at a fabric edge. No powered actuator, network, or contact with hazardous voltages is used.

## Hardware
Controller: Raspberry Pi Pico (RP2040), USB 5 V input, 3.3 V GPIO. Sensor: TCRT5000 reflective IR module operated from Pico 3V3; analog output to GP26/ADC0. Set its trim pot so the bare fixture is below 1.0 V and a deliberately inserted fiber near the comb crosses the threshold; analog output must remain within 0–3.3 V. A 1 kΩ series resistor and 100 nF capacitor at GP26 provide input protection/filtering. GP14 is a start/stop button to ground; GP15 is a mark-length button to ground; both use internal pull-ups. Sensor ground and Pico ground are common. Use the module's analog output only; do not use a 5 V-powered module.

Power: USB supplies Pico and sensor; design budget ≤100 mA, no battery. The comb is a passive rounded nylon brush secured to a printed fixture; keep fingers clear of sharp sample edges. No PCB is supplied. This is a reference design, not fabricated or physically validated.

See `schematic/netlist.md` for the authoritative wiring schedule, `hardware/BOM.csv` for parts, and assembly guide for setup. The text wiring schedule is not a native KiCad schematic.

## Firmware
C11 with Raspberry Pi Pico SDK 2.1.1. Firmware application core is portable C and host-buildable: `cmake -S firmware -B build && cmake --build build && ctest --test-dir build`. A Pico SDK adapter is documented but not included; this repository version emits sample ADC and button handling through a hardware adapter interface, so it is not yet a flashable Pico image. This limitation is explicit.

At each valid shedding pulse, JSONL output contains test number, accumulated fiber events, and elapsed milliseconds. Calibrate the threshold per sensor/sample. Event count depends on fabric, comb geometry, humidity, speed, sensor height, and threshold. Compare only repeated runs with fixed conditions; do not interpret as standardized test result.

Author: jayis1.
