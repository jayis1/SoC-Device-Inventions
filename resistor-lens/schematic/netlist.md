# Connectivity record (reference only)

Not a KiCad capture. Use Pico board and a TCS34725 breakout configured for 3.3 V logic.

| Net | Endpoints | Notes |
|---|---|---|
| VBUS | Pico USB connector, board | USB 5 V input |
| 3V3 | Pico 3V3 output, sensor VCC, R1 | Sensor rail |
| GND | Pico GND, sensor GND, Q1 source, R2 low side | Common ground |
| I2C0_SDA | Pico GP4, sensor SDA | 0x29, 3.3 V pull-up on breakout |
| I2C0_SCL | Pico GP5, sensor SCL | 0x29, 3.3 V pull-up on breakout |
| LED_GATE | Pico GP6, R2, Q1 gate | R2 pulldown |
| LED_K | LED1 cathode, Q1 drain | Low-side switch |
| LED_A | 3V3 through R1, LED1 anode | R1 330 ohm |
| UART0_TX/RX | Pico GP0/GP1, debug header | Optional 3.3 V only |

Q1 is a 2N7002 N-channel MOSFET. Verify its pinout for the chosen package. Sensor board and Pico footprints are module-level and intentionally not specified as a fabrication-ready PCB.
