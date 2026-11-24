# Fold Stopwatch

Fold Stopwatch is an offline, USB-powered timing aid for paper-folding workshops. Two optical interrupters on a small fixture timestamp the leading and trailing edges of a sheet as it passes a fold station; the device reports elapsed passage time and inter-pass intervals over USB CDC. It answers a narrow process question—whether a manual feed/fold cadence is changing—without controlling a press or judging fold quality.

## Architecture

```text
USB 5 V -> Raspberry Pi Pico (RP2040)
  GP2 <- gate A phototransistor output
  GP3 <- gate B phototransistor output
  GP25 -> onboard status LED
  USB CDC -> CSV log
```

The Pico is a module (SC0915), not a bare-chip design. GP2/GP3 are digital inputs with internal pull-ups. Each gate is an open-collector phototransistor module powered from 3V3, with output pulled up to 3V3 by 10 kΩ. Gate A and B should be mounted in order along the sheet path; fixed spacing is not used in the reported timing. Firmware timestamps each alternating A/B event using a monotonic microsecond clock. The host can compute speed only if spacing is measured separately.

| Pico pin | Function | Net |
|---|---|---|
| GP2 | gate A digital input | J2.OUT, R1 pull-up |
| GP3 | gate B digital input | J3.OUT, R2 pull-up |
| 3V3(OUT) | sensor supply and pull-ups | J2.VCC, J3.VCC, R1/R2 |
| GND | common return | J2/J3.GND |
| GP25 | onboard LED | event/status indication |
| USB | power + serial | Pico connector |

## Power and BOM

USB powers the Pico and low-current optical gates. No battery, radio, motor, mains wiring, or safety control is present. Verify the selected gate modules are 3.3 V compatible; do not use a module whose output can rise to 5 V.

| Ref | Qty | Component | Notes |
|---|---:|---|---|
| U1 | 1 | Raspberry Pi Pico SC0915 | RP2040 module |
| Q1-Q2 | 2 | phototransistor + IR LED interrupter, e.g. Vishay TCST2103 | Through-hole slot sensor; emitter/current limiting per datasheet |
| R1-R2 | 2 | 10 kΩ | Input pull-ups to 3V3 |
| C1-C2 | 2 | 100 nF | Sensor local decoupling |
| J1 | 1 | USB cable | Power and CDC |
| J2-J3 | 2 | 3-pin header | 3V3, OUT, GND sensor ports |

Use sensor manufacturer datasheets to confirm pin order and LED current-limiting requirements before assembly. The BOM is a reference, not a sourcing guarantee.

## Firmware and output

The portable C core records event transitions, rejects repeated same-gate edges, and computes bounded inter-event durations. A board adapter should initialize GPIO pull-ups and use `time_us_64()` from Pico SDK 2.x, then emit `sequence,gate,elapsed_us` via USB CDC. The source intentionally isolates this event logic from hardware APIs.

Host compile check:

```sh
cc -std=c11 -Wall -Wextra -Werror -I firmware/include -c firmware/src/fold_stopwatch.c -o /tmp/fold_stopwatch.o
```

No Pico SDK or KiCad CLI was available in the execution environment. No target build, actual schematic/ERC, or physical test is claimed. `schematic/netlist.md` is a wiring reference only.

## Assembly and test

Mount gates in a rigid bracket with the sheet path crossing both slots. Keep the sensors shielded from direct room light. Wire only 3.3 V logic. Confirm GP2/GP3 idle high and transition low when a card interrupts the beam. Pass a card through gate A then B; verify the event order and monotonic timestamps, then pass it in reverse and confirm the firmware reports reverse order rather than a fabricated speed. Test 100 passes and inspect for missed/double transitions before using the log. No dimensional or timing accuracy has been characterized.

## Limitations

This is a reference prototype design, not a fabrication-ready PCB, calibrated instrument, or machine interlock. Gate alignment, ambient-light rejection, edge debounce, and actual repeatability require physical testing. It measures optical interruption timing only; it cannot identify a fold or assess paper quality.

## License

MIT. Authored by jayis1.
