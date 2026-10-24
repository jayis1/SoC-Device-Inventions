# Stitch Sentinel

## Why this exists
Home sewists and repair cafes often discover a skipped stitch or broken upper thread only after a long seam. Existing machines vary widely in built-in thread sensing; generic stitch counters count needle motion but cannot distinguish a stitch cycle with no fabric advance. Stitch Sentinel is an external, non-invasive retrofit: it compares needle-cycle pulses with fabric-feed motion and flags sustained mismatches. Its defining capability is the paired, machine-independent cycle/feed check; it neither modifies a sewing machine nor claims to identify every stitch defect.

First ten minutes: attach the reflective feed marker to the fabric, clip the optical sensor near the needle bar, connect USB, and observe `CYCLE`/`FEED` CSV events; deliberately stop advancing fabric to see the mismatch alarm.

Closest repo neighbor is Warp Watch, which measures loom warp tension/take-up on RP2040. This design instead monitors a household sewing operation using optical cycle and fabric-motion sensing; it does not measure textile tension or loom motion.

## Design and limits
Prototype reference design; hardware unverified. Intended for indoor domestic machines only, powered from isolated 5 V USB. No connection to motor, mains, foot pedal, or machine wiring. Optical geometry depends on the machine and fabric; setup calibration is essential. It detects an operational proxy, not seam quality, and can alarm on intentional pauses or difficult fabrics.

SoC: Raspberry Pi Pico (RP2040). Chosen for two independent edge-capture inputs and simple USB serial, without a radio or cloud requirement; an ESP32-C3 would add unnecessary radio complexity. Firmware targets Pico SDK 2.x.

## Block diagram

```text
Needle-bar reflective marker -> TCRT5000 module -> GPIO2 (cycle edges) ┐
Fabric marker/roller          -> TCRT5000 module -> GPIO3 (feed edges)  ├-> RP2040 -> USB CSV + buzzer + status LED
USB 5 V -> Pico VSYS/3V3 -> sensors and GPIO (3.3 V logic)             ┘
```

## Pin assignments

| Pico GPIO | Function | Notes |
|---|---|---|
| GP2 | Needle-cycle digital input | External 10 kΩ pull-up to 3V3; falling edge |
| GP3 | Feed-motion digital input | External 10 kΩ pull-up to 3V3; falling edge |
| GP14 | Piezo buzzer via 2N7002 low-side driver | 100 Ω gate resistor, 100 kΩ pulldown |
| GP15 | Status LED via 1 kΩ | Active high |
| USB | Power and serial | Isolated 5 V supply only |

No Pico QSPI, boot, SWD, or ADC pins are repurposed. Sensor modules must be powered at 3.3 V and their output must not exceed 3.3 V. Verify module output polarity before connecting.

## Power architecture
USB 5 V from a certified isolated supply to Pico VSYS; Pico onboard regulator supplies 3V3. Estimated: Pico 25–40 mA (USB active), two reflective sensors 2×10 mA, buzzer up to 15 mA, LED 2 mA; design peak under 80 mA. No battery/runtime claim. This is an estimate, not a measured budget. Do not power sensor LEDs from GPIO.

## Firmware
Interrupt handlers timestamp edge events using the RP2040 microsecond timer. A 250 ms activity window compares cycle and feed counts; three consecutive cycle events without feed motion raise a warning. A 20 ms debounce rejects optical chatter. USB CDC emits `ms,event,cycles,feeds,alarm` comma-separated lines; host is the zero-infrastructure data path. No automatic machine control.

Build with Pico SDK 2.x and CMake: configure with `PICO_SDK_PATH` set, then build the `firmware` directory. See [assembly](docs/assembly.md), [serial API](docs/api.md), [netlist](schematic/netlist.md), and [BOM](hardware/BOM.csv). Firmware is a reference implementation and has not been built or hardware-tested in this run.

## Calibration and use
With machine unplugged, position the cycle sensor over a reflective marker on a safe moving needle-bar surface and the feed sensor at a fabric-driven roller or marked fabric edge. Keep all sensors and leads clear of the needle, presser foot, handwheel, belt, and motor. Power only after securing the assembly. Sew slowly on scrap and tune sensor threshold/geometry until idle and active edges are stable. Confirm a known deliberate pause does not cause unacceptable nuisance alarms.

## Safety and limitations
Never attach anything to live or motor circuits. Keep the retrofit physically clear of moving parts and the needle; stop and unplug the sewing machine before adjusting it. Use low-voltage USB only. This device is advisory and must not be used as a safety interlock. There is no claimed accuracy, universal compatibility, or fabric defect diagnosis.

## Validation status
The source files, pin map, BOM, netlist, and documentation are a design reference, not a fabrication-ready KiCad project. No KiCad CLI, Pico SDK, hardware, or measured test results are available in this run. Do not treat the design as tested or production-ready.

Author: jayis1.
