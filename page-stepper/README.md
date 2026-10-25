# Page Stepper

A wired, two-input USB foot controller for hands-free page navigation in sheet music, recipes, teleprompters, and computer use. It enumerates as a USB HID keyboard and sends Page Up/Page Down; no radio, account, driver, or host application is required.

## Product brief and novelty

Musicians and cooks need page navigation while both hands are occupied. This inexpensive controller provides two durable low-voltage foot inputs without camera tracking or a proprietary wireless receiver. Success target: a host receives standard HID Page Up/Down within 50 ms of a stable pedal press. Target parts cost US$12–20; assembly 1–2 hours. Non-goals: certification, wireless use, host-specific application integration.

This is a wired human-interface controller, unlike the repo's environmental sensors and scientific instruments: no measurement, logging, radio, or data analysis. It uses the RP2040 USB device peripheral and mechanical dry contacts.

## SoC and architecture

Raspberry Pi Pico (RP2040) is selected for native USB device support and integrated bootloader, avoiding custom QSPI/clock design. Two normally-open pedals connect to GPIO inputs with pull-ups. USB powers the board and carries HID reports.

```text
[Pedal A]--GP14 pull-up--+      +--USB HID keyboard--> Host
[Pedal B]--GP15 pull-up--+----> RP2040
[USB 5 V]--> Pico regulator      +--GP25 onboard LED
```

## Pin map

| Pico signal | Function | Notes |
|---|---|---|
| GP14 | Previous page | Active-low, internal pull-up |
| GP15 | Next page | Active-low, internal pull-up |
| GP25 | Status LED | Onboard Pico LED |
| GND | Pedal common | Dry contacts only |
| USB | Power and HID | Pico USB connector |

No pins are shared; GP14/15 avoid flash, SWD and USB pins. Pedal wiring is low-voltage and isolated.

## Power

USB 5 V is converted by the Pico module's onboard regulator to 3.3 V. Board demand is expected below 50 mA but must be confirmed by measurement. No battery or charger is used. Never connect external voltage to pedal inputs.

## BOM

See hardware/BOM.csv. Cost figures are planning estimates, not verified quotes. Use a genuine Raspberry Pi Pico board. No custom PCB is supplied; wiring is point-to-point.

## Firmware and build

Targets Pico SDK 2.1.1 and TinyUSB HID. Set PICO_SDK_PATH, then run `cmake -S firmware -B firmware/build -DPICO_BOARD=pico` and `cmake --build firmware/build`. Copy `page_stepper.uf2` to the board in BOOTSEL mode. A debounced press sends a key down/up pair. Host key-repeat behavior is outside device control.

## Assembly and bring-up

1. With USB disconnected, wire each momentary switch between its GPIO and GND.
2. Verify both contacts are open at rest and carry no external voltage.
3. Flash UF2 and connect USB; confirm the host lists a USB keyboard.
4. Test each pedal in a text editor or browser.
5. Stop if the board/cable heats or USB disconnects repeatedly; inspect for 3V3/GND shorts.

## Protocol

Standard USB HID keyboard: left pedal Page Up, right pedal Page Down. No network, data storage, or configuration API.

## Validation status and limitations

Host-side compiler and Pico SDK/TinyUSB compilation, USB enumeration, KiCad ERC, and physical operation have not been performed. `schematic/netlist.md` is the reference connection list, not a KiCad capture or fabrication-ready PCB. Current and latency are targets, not measured results.

## Safety and license

USB low-voltage only; never attach pedals to mains or powered interfaces. MIT. Designed by jayis1.
