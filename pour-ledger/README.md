# Pour Ledger

Offline pour-over brew scale and process logger for a home barista who wants repeatable hand pours without a phone on the wet counter. It measures mass from a 1 kg load cell, estimates incoming flow, marks recipe stages, and records an auditable USB CSV log. Success target: stable readings within ±2 g over 0–1000 g after calibration, and a live flow estimate updated at 5 Hz. The phone app is unnecessary for tare, timing, stage prompts, and local logging; a kitchen scale generally does not expose a pour-rate trace or staged brew record.

This is distinct from repository instruments: it is a low-voltage kitchen workflow device, not a chemical, environmental, or structural measurement instrument. Design-space position: food preparation / mass-flow interaction / wired USB / countertop / USB power. Novel insight: turn the familiar scale into a time-aligned pour-rate instrument with stage markers, rather than adding connectivity or a sensor dashboard.

## Hardware and status

RP2040 Raspberry Pi Pico (Pico SDK 2.1.1), HX711 24-bit bridge ADC module, 1 kg single-point load cell, 128×64 SSD1306 I2C OLED, two buttons, USB-powered. No battery, radio, mains connection, or liquid-contact electronics. Keep the electronics enclosure above and outside the spill tray; this is not a waterproof or food-contact product.

```text
Load cell -> HX711 -> Pico RP2040 -> USB CDC (CSV)
                         |          -> SSD1306 OLED
                         + buttons (tare / mark)
```

Pin assignments (Pico GPIO numbers): HX711 DOUT GP14, SCK GP15; OLED SDA GP4/SCL GP5 (I2C0, address 0x3C); tare GP10 to ground; mark GP11 to ground. Buttons use internal pull-ups; HX711 SCK defaults low. USB is the only power input. Pico 3V3 powers HX711 logic and OLED; load-cell excitation is 3V3 from the HX711 module. Verify the module's regulator/jumper configuration: do not feed 5 V logic to Pico GPIO. GPIO15 is not a boot strap on RP2040; avoid driving it high during reset.

Power budget (design estimate): Pico up to 50 mA, OLED 25 mA typical / 35 mA bright, HX711 and bridge up to 10 mA combined; budget 100 mA from USB. No battery-life claim. Estimated parts cost US$20–35 including Pico, load cell, HX711, display, buttons, platform, and enclosure; estimate only. Build difficulty: easy through-hole/module assembly; soldering required for headers and load-cell wiring.

## Firmware

`firmware/` contains Pico SDK C source. PIO samples HX711 data in a bounded state machine; an EMA-filtered mass value, 1-second rolling flow-rate estimate, elapsed timer and recipe stage events are reported. Recipe target stages are configurable in `config.h`. Missing/stuck-high HX711 data triggers a visible `SENSOR ERROR`, not fabricated readings. CSV fields: `ms,mass_g,flow_g_s,event`. Build with Pico SDK 2.1.1 and CMake/Ninja per `firmware/README.md`. Firmware is reference source and has not been cross-built or hardware-tested in this environment.

## Schematic and validation

`schematic/netlist.md` specifies the complete connectivity. A native KiCad schematic is not supplied yet; schematic is an explicit wiring/netlist reference, not fabrication data. No PCB is supplied. Verification status: design reviewed; firmware build, KiCad ERC, calibration accuracy, and physical assembly are unverified. Do not represent readings as laboratory measurements; calibration with known masses is required.

## Prior art and what is new

Coffee scales such as Acaia Pearl and Timemore Black Mirror provide mass and timers; generic USB load-cell scales provide mass output; Brewfather and phone timers provide recipes. This design is an open, wired reference build whose primary output is synchronized mass, flow, and manually marked pour stages as plain CSV, with no account, network, or app required. It does not claim novelty over all prior art.

## Bring-up

Inspect load-cell wire colors and the specific cell datasheet before connecting; color conventions vary. With the platform unloaded, power from a current-limited USB source (limit 200 mA), verify 5 V and 3.3 V rails, then connect the sensor and confirm zero stability. Calibrate using documented known masses, check tare and stage buttons, and confirm USB CDC CSV output. Stop if the board or HX711 heats, rails collapse, or readings drift with no load. Keep liquid away from electronics; wipe only the removable platform.

See [assembly guide](docs/assembly.md), [API reference](docs/api-reference.md), [netlist](schematic/netlist.md), and [BOM](hardware/BOM.csv). Author: jayis1.