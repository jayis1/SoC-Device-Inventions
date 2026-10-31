# Hinge Meter

USB-powered fixture that compares the opening torque of small hinges and lids by measuring the force applied at a known lever arm. Intended for hobbyist product comparison and repeatable incoming inspection—not certified metrology.

## Why this device

A spring scale gives a subjective peak reading and does not preserve the force-versus-angle trace. Hinge Meter samples a load cell and a rotary encoder together, then exports raw angle/force records for comparison. Unlike the repo's torque-pebble (fastener torque-angle) and knot-witness (tensile pull), it characterizes slow rotational resistance across a repeatable arc. Its three novelty axes are consumer-product mechanism testing, RP2040 USB MCU, and load-cell-plus-encoder synchronized sweep.

## Design target and limitations

0–5 N force at a 100 mm lever arm (0–0.5 N·m), manual sweep up to 90 degrees, 20 Hz records. Resolution depends on fixture stiffness and calibration; no accuracy claim is made before calibration. USB 5 V only; no battery, radio, motor, or exposed hazardous voltage. A 1 kg load cell is deliberately operated below its rated capacity. Operator must move the lever slowly and keep fingers clear of pinch points.

## Block diagram

```text
USB 5 V -> Raspberry Pi Pico (RP2040) -> USB CDC records
                         | GPIO6/7 SPI
                  HX711 load-cell ADC <- 1 kg strain-gauge bar
                         | GPIO10/11/12
                  quadrature encoder <- hand-driven lever
```

## Pin map

| Signal | Pico GPIO | Interface |
|---|---:|---|
| HX711 DOUT | 6 | input |
| HX711 SCK | 7 | output; keep low between reads |
| Encoder A, B, pushbutton | 10, 11, 12 | GPIO inputs with pull-ups |
| USB CDC | USB connector | Pico native USB |

No GPIO is shared. RP2040 GPIO 0–29 support GPIO; this design avoids the board's flash-connected pins by using the Pico module and only its exposed GPIO header. Confirm the particular Pico revision pinout before wiring.

## Power

Pico via USB 5 V; onboard 3.3 V rail powers HX711 and encoder. Approximate design budget: Pico 20–40 mA, HX711 <2 mA, encoder <1 mA; under 50 mA expected. USB current is not a battery-life claim. HX711 analog readings require zero and span calibration using known weights; the encoder provides relative angle, not absolute fixture alignment.

## Firmware and protocol

C firmware uses Pico SDK 2.x, SPI-mode HX711 bit-banging, quadrature edge counting, and USB CDC. Each completed sample is emitted as CSV: `ms,angle_deg,raw_adc,force_n`. The firmware records raw ADC values; conversion to newtons requires the calibration factor entered in the host helper. Build with `cmake -S firmware -B firmware/build -DPICO_SDK_PATH=/path/to/pico-sdk`, then `cmake --build firmware/build`. Flash the UF2 by holding BOOTSEL while connecting USB and copying the generated UF2 to RPI-RP2.

## Readiness

Reference design, not fabricated. This repository lacks the Pico SDK and ARM cross-compiler, so target firmware build and hardware behavior are unverified. Schematic is a connection-level netlist, not native KiCad capture; no ERC has been run. Validate the exact Pico/HX711 module pinouts, check load-cell polarity, and calibrate with at least two known weights before relying on results. MIT license, consistent with repository.
