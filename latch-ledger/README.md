# Latch Ledger

USB-powered cabinet-latch cycle counter and opening-duration logger for furniture makers and repair shops comparing latch samples over repeated manual or fixture-driven cycles. Unlike the repository's hinge-sag fixture, it counts magnetic open/close transitions and records dwell duration; it does not measure hinge geometry or force.

## Key insight

Use a latching Hall switch with a magnet on the moving leaf, then debounce and classify transitions in firmware. This measures the physical state without wiring into a latch, loading a hinge, or placing a switch in its travel path.

## Block diagram

```text
5 V USB -> Raspberry Pi Pico (RP2040) -> USB CDC JSONL -> host logger
                         GPIO2 <- DRV5032FC Hall switch (open-drain)
                         GPIO25 -> onboard status LED
```

The sensor is a DRV5032FC low-power, omnipolar, open-drain Hall switch powered at 3.3 V. Verify the exact suffix/pinout and output type for the purchased variant before assembly. External magnet and switch need a rigid adjustable bracket; sensor gap and polarity are fixture-specific.

## Pin map

| Pico signal | Function | Notes |
|---|---|---|
| GP2 (physical pin 4) | Hall OUT, active-low | 10 kΩ pull-up to 3V3; 100 nF sensor bypass |
| 3V3(OUT) (pin 36) | Hall VDD | Never connect sensor to VBUS |
| GND (pin 38) | Sensor return | Common ground |
| GP25 | On-board LED | Status only |
| USB | Power + CDC | Use a data-capable cable; no battery |

GP2 is not a Pico flash/USB pin or RP2040 ADC input. No RP2040 boot-strap configuration is changed by this assignment. Keep the Hall output open-drain and 3.3 V pulled up; do not apply 5 V to GPIO.

## Power and safety

USB 5 V powers the Pico; its regulated 3V3 rail powers only the Hall switch. No charging circuit or exposed actuator is present. Estimated Hall current is sub-mA; board load is dominated by the Pico and varies with USB activity. Runtime is therefore not specified. Prototype is low-voltage, non-safety-critical, and has not been EMC or USB-IF tested. Keep the sensor wiring short and strain-relieved.

## BOM

See `hardware/BOM.csv`. Prices and availability are not asserted. Use the Pico with its USB connector and onboard regulator intact. The magnet, fixture and USB cable are not included in the electronic BOM.

## Firmware build

Pico SDK 2.x, CMake, and an ARM GCC toolchain are required. Set `PICO_SDK_PATH`, then from `firmware/` run `cmake -S . -B build` and `cmake --build build`. Copy `build/latch_ledger.uf2` to the Pico in BOOTSEL mode. No SDK/toolchain is installed in the authoring environment; target compilation is unverified. `sdkconfig.defaults` is not applicable to Pico SDK.

Firmware emits one JSON object per line over USB CDC: `{"event":"open","ms":1234}`; closure records include `dwell_ms`, and startup emits a version record. The event time is a monotonic boot-relative millisecond count, not wall time. Debounce rejects transitions closer than 30 ms. A reset loses the current open interval; no persistent storage is claimed.

## Assembly and calibration

1. Power down. Connect the sensor per `schematic/netlist.csv`; add the 10 kΩ pull-up and local 100 nF bypass.
2. Mount the magnet on the moving door and Hall switch on the fixed frame, initially with a 5–10 mm gap. Secure all wiring away from pinch points.
3. Flash firmware and open its USB CDC port at 115200 (baud is ignored by native USB CDC).
4. Move the door slowly through its full travel. Confirm exactly one `open` and one `close` event per cycle. Adjust alignment to avoid chatter or missed transitions.
5. Validate counts against a manually recorded 20-cycle run. This is a user calibration procedure, not a performed test.

## Host helper

`scripts/summarize.py` reads JSONL and reports valid open/close cycles and dwell-time statistics. Run `python3 scripts/summarize.py --help`; a fixture supports hardware-free parser testing.

## Validation status

Reference design only; not fabricated or physically validated. BOM CSV and host helper are syntax/fixture checked. KiCad is unavailable, so schematic is an explicit component/net connection list rather than a pretend KiCad file. Firmware target build remains unverified because Pico SDK and ARM GCC are unavailable.

## License

MIT. Invented and maintained by jayis1.
