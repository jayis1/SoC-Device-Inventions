# Resistor Lens — contact color-band reader

A USB-powered bench aid that estimates the value of a conventional 4-band axial resistor from a controlled RGB illumination and color-sensor reading. Intended for makers sorting loose parts; it is not a calibrated instrument. It differs from dial-reader by reading a small coded color sequence rather than the angle of a pointer.

## Block diagram

```text
Pico USB 5 V -> board 3V3 -> TCS34725 RGB sensor (I2C)
                           -> white LED + current-limit resistor
RGB clear-channel samples -> RP2040 -> USB CDC JSONL
```

## SoC and implementation

Raspberry Pi Pico (RP2040) was selected for its USB CDC support, accessible GPIO/I2C, and convenient 3.3 V board supply. The design uses the Pico board as a module, not a bare RP2040 schematic. Firmware acquires color samples and emits raw channels; an optional host script decodes four manually aligned bands. It deliberately does not claim automated band segmentation or calibrated accuracy.

## Pin map

| Pico GPIO | Signal | Connection |
|---|---|---|
| GP4 | I2C0_SDA | TCS34725 SDA |
| GP5 | I2C0_SCL | TCS34725 SCL |
| GP6 | LED_EN | white LED cathode switch via 2N7002; 100 kΩ gate pulldown |
| GP0/GP1 | UART0 | optional 3.3 V debug |
| USB | CDC | power and newline-delimited output |

I2C address is 0x29. Use a breakout whose pull-ups connect only to 3.3 V; avoid duplicate strong pull-ups. All Pico GPIOs are 3.3 V only. Do not install the optional sensor LED on the module; the external white LED is current-limited.

## Power and safety

USB 5 V supplies Pico; Pico 3V3 supplies the sensor. LED current is limited by 330 Ω: nominal current is approximately (3.3 V − 2.8 V)/330 Ω = 1.5 mA, depending on LED forward voltage. Board and sensor current is USB-powered; no battery/runtime claim. This is a low-voltage, non-safety device.

## BOM

See `hardware/BOM.csv`. Estimated costs are indicative only, not a live quote. The sensor module is a breakout, so verify its regulator and pull-up configuration before wiring.

## Firmware / protocol

Pico SDK 2.1.1 project in `firmware/`. On start it initializes I2C, enables illumination and reports errors over CDC instead of hanging. Each sample is JSON: `{"clear":N,"red":N,"green":N,"blue":N}`. Build with `cmake -S firmware -B build -DPICO_SDK_PATH=/path/to/pico-sdk -DPICO_BOARD=pico && cmake --build build` and flash the resulting UF2 by BOOTSEL. The host helper accepts four normalized dominant-band labels and calculates the nominal IEC 60062 4-band value; run `python3 scripts/decode_bands.py --help`.

## Schematic status

`schematic/netlist.md` records connectivity. It is not a KiCad schematic and has not had ERC/DRC. Confirm breakout pinout and sensor illumination settings before assembly.

## Verification and limitations

Firmware build: not run (Pico SDK unavailable in this environment). Hardware: not fabricated or tested. Sensor readings depend on distance, ambient light, resistor coating, and alignment; this prototype is an assistive sorting aid only. No color calibration or fifth-band/tolerance interpretation is implemented. Designed and documented by jayis1.
