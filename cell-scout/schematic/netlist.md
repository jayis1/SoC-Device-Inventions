# Cell Scout connection list (not a KiCad schematic)

| Net | Connections |
|---|---|
| USB_5V | Pico USB input only; not connected to cell positive |
| CELL_POS | Holder positive, R1 100 ohm pin 1, R2 100k pin 1 |
| LOAD_NODE | R1 pin 2, Q1 drain |
| CELL_NEG | Holder negative, Q1 source, Pico GND, R3 100k pin 2 |
| ADC_SENSE | R2 pin 2, R3 pin 1, Pico GP26/ADC0 |
| LOAD_GATE | Pico GP15 through R4 100 ohm, Q1 gate; R5 100k from gate to source |

Use a single-cell CR2032 holder and a 3.3 V-gate-compatible N-MOSFET. Confirm the selected MOSFET pinout from its datasheet; package pin numbers vary. Do not infer PCB footprints from this connection list.