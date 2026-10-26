# Film Gate

A USB-powered, non-networked 35 mm film digitizing gate that synchronizes a camera trigger to a manually advanced film frame and checks for double advances. Persona: a small-archive volunteer digitizing negatives with a copy stand; each accepted frame receives one isolated shutter pulse, avoiding missed or duplicated captures.

This device fills the gap between a passive film holder and expensive motorized scanners: it does not transport film or process images. It senses the frame edge optically and presents a debounced foot-switch trigger. It is a reference design, not a validated camera accessory.

## Features and non-goals

- Detects opaque frame gaps with a reflective optical sensor; a foot switch requests one exposure only while film is stationary.
- Open-drain optocoupler output provides a camera remote-contact closure; camera-specific cable pinout must be verified by the builder.
- USB CDC reports `READY`, `BUSY`, `ADVANCE`, `SHOT`, and `FAULT` records.
- Non-goals: powered film transport, image capture, mains operation, wireless control, or universal camera compatibility.
- Build difficulty: through-hole / breakout-board prototype; estimated parts cost USD $18–30, excluding camera and copy stand (rough estimate, not a live quote).

## Block diagram

```text
Film gate -> reflective sensor -> RP2040 -> USB CDC host log
Foot switch --------------------^   |
Camera remote contacts <- optocoupler
USB 5 V -> Pico board regulator -> 3.3 V logic
```

## SoC choice

Raspberry Pi Pico uses RP2040, whose PIO-capable GPIO and USB device controller make deterministic edge capture and USB CDC possible without a radio stack. Firmware targets Pico SDK 2.x; exact SDK/toolchain builds have not been run here.

## Pin assignments

| Signal | Pico GPIO | Notes |
|---|---:|---|
| Optical sensor digital output | GP14 | 3.3 V logic only; sensor board output must be verified |
| Foot switch | GP15 | Normally open to ground; internal pull-up |
| Optocoupler LED control | GP16 | Series resistor; active high |
| USB CDC | USB D+/D− | On-board USB connector |

No I2C or SPI devices are used. GP14–16 are ordinary RP2040 GPIO, not ADC pins; SWD remains available on board pads.

## Power architecture and budget

USB 5 V powers the Pico board; onboard regulator supplies 3.3 V. No battery, charger, or external supply. Estimated current: Pico 20–35 mA active, sensor module up to 20 mA, optocoupler LED 5 mA during shutter pulse; budget 60 mA from USB. Verify the selected sensor breakout's voltage and current before connection. A 330 Ω series resistor limits LED current from GP16. Do not connect camera voltage to Pico GPIO; only the optocoupler's isolated transistor contacts reach the camera cable.

## BOM

See [hardware/BOM.csv](hardware/BOM.csv). MPNs identify common parts; availability and pricing are not guaranteed. Sensor module includes an LM393 comparator and adjustable threshold; calibrate against actual film and gate geometry.

## Firmware architecture

`firmware/src/main.c` implements a simple state machine with a 30 ms switch debounce and sensor-stability check. The camera output is a 150 ms optocoupler pulse, then a 500 ms lockout. Fault/ready events are emitted over USB CDC. A real camera trigger is deliberately opt-in at build time (`FILM_GATE_ENABLE_SHUTTER`), defaulting to disabled until cable polarity and camera compatibility are confirmed.

## Build and flash

Install Raspberry Pi Pico SDK 2.x and its documented ARM GCC/CMake/Ninja dependencies. Set `PICO_SDK_PATH`, then run `cmake -S firmware -B firmware/build -DPICO_BOARD=pico` and `cmake --build firmware/build`. Flash the UF2 using the BOOTSEL USB mass-storage procedure. These instructions are unverified in this environment; hardware behavior is not tested.

## Assembly

Mount the sensor so the film base passes its optical field with repeatable spacing; adjust the comparator threshold to distinguish image area from the clear inter-frame gap. Test USB logging and foot-switch behavior before connecting a camera. Use a multimeter to confirm the optocoupler output is electrically isolated from Pico ground. Do not attach an unverified remote cable.

## API

USB CDC emits newline-terminated ASCII events: `READY`, `BUSY`, `ADVANCE`, `SHOT`, or `FAULT,<reason>`. Host software should treat `SHOT` as a request event, not proof that the camera exposed.

## Schematic and status

[Connection-level schematic/netlist](schematic/netlist.md) is the authoritative reference; it is not a KiCad schematic and has not undergone ERC. Firmware is reference code and has not been compiled or tested on hardware. Optical performance depends on gate alignment and film stock.

## Safety and prior art

Low-voltage USB device; no medical or mains function. The optocoupler protects against camera-side voltage only when wired as specified. Unlike passive holders (e.g. Essential Film Holder) this adds frame-advance event logging and an isolated exposure request; unlike motorized scanners it leaves film handling manual. Camera remote interfaces vary, so verify the exact model's trigger requirements.

## License

MIT; attribution: jayis1.
