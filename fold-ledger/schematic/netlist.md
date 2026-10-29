# Fold Ledger connection specification

Not a native KiCad schematic. Capture these nets in KiCad before board fabrication.

| Net | From | To | Notes |
|---|---|---|---|
| USB_5V | Pico USB connector | Pico onboard power path | Use Pico module as supplied |
| GND | Pico GND | foot switch return, optocoupler emitter, LED return | Common only on low-voltage side |
| FOOT_N | Pico GP14 | normally-open switch to GND | Internal pull-up enabled |
| OPT_N | Pico GP15 | PC817 collector; 10 kΩ pull-up to 3V3 | PC817 emitter to GND |
| STATUS | Pico GP16 | 1 kΩ then LED anode; cathode to GND | Active-high indicator |
| ISO_IN+/- | External isolated sensor output | PC817 LED through calculated resistor | Must remain galvanically isolated; design input resistor for the actual SELV sensor voltage |

The external sensor and its supply are unspecified and are not part of the device BOM. Do not wire this input to machine mains or non-isolated control circuitry. This specification is not a fabrication-ready schematic or PCB layout.
