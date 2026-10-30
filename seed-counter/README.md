# Seed Counter

Conceptual, offline seed-packet counting aid for community seed libraries and small growers who need to divide mixed-size dry seeds into repeatable packet counts without cloud services. A falling seed interrupts a modulated optical beam; the operator pauses, inspects the count, and bags the batch. This is a counting aid, not a precision dispenser.

Unlike the repository's grain moisture analyzer, this tool counts individual falling objects rather than estimating bulk material properties. Signature trick: modulated IR plus synchronous on/off sampling rejects ambient light; an adjustable opaque chute limits double counts.

## Architecture

```text
USB 5 V -> Pico board -> 3V3
  GPIO2/3 I2C -> 128x32 OLED
  GPIO14 -> 38 kHz IR LED driver -> chute
  GPIO15 <- phototransistor amplifier <- chute
  GPIO16 -> buzzer (optional)
  GPIO17 <- reset button
```

Use Raspberry Pi Pico (RP2040). USB-powered, no battery or wireless. Use a 220 ohm resistor for IR LED and transistor driver; never drive an IR LED directly from GPIO. Sensor output must stay within 0–3.3 V. OLED I2C address 0x3C, 3.3 V pullups. Reserve GPIO0/1 for UART/debug if desired; SWD remains available on the board.

## Power

USB 5 V input via Pico; its onboard regulator supplies 3.3 V. Estimated active load: Pico 25–40 mA, OLED 5–15 mA, IR LED pulsed average 5–15 mA, buzzer <10 mA; budget USB supply for 100 mA. No sleep-runtime claim (USB powered). Validate actual current on prototype.

## BOM

See hardware/BOM.csv. Component selections are representative; verify footprints and resistor values on the prototype.

## Build

Firmware is portable C counting logic; this first reference drop is not a complete Pico SDK application. Build/test core with `gcc -std=c11 -Wall -Wextra -Werror -c firmware/src/seed_counter.c -o /tmp/seed_counter.o`. Hardware integration, display driver, and calibration remain unimplemented. Not a fabrication-ready schematic; schematic/netlist.md is the wiring source. No PCB is supplied.

## Known limitations

No optical geometry has been experimentally validated. Seeds may bounce, overlap, or block the beam; count accuracy depends on chute geometry and calibration. Not suitable for coated/dusty seed without cleaning. Reference design only; hardware unverified.
