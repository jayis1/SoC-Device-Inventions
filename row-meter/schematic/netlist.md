# Row Meter point-to-point schematic (reference)

This file is the authoritative connection drawing in text form; no KiCad schematic capture is included. U1 is a Raspberry Pi Pico board, not a bare RP2040.

- J1 USB on Pico: 5 V input and USB CDC; no other supply.
- U1 3V3(OUT) pin 36 → OLED VCC, optical interrupter VCC; GND pins → common GND.
- GP4 → OLED SDA; GP5 → OLED SCL. OLED module address 0x3C; use a 3.3 V module and its onboard pull-ups (or fit 4.7 kΩ SDA/SCL pull-ups to 3V3, not both).
- GP10 → optical channel A; GP11 → channel B. Sensor outputs must be push-pull or externally pulled up to 3V3. 100 nF at sensor VCC/GND.
- GP14 → MARK pushbutton → GND; firmware internal pull-up.
- GP15 → R1 4.7 kΩ → Q1 2N3904 base; Q1 emitter → GND; piezo positive → 3V3, piezo negative → Q1 collector. R2 100 kΩ base-to-GND. Use a passive piezo transducer, not a magnetic buzzer.
- Test points: TP1=3V3, TP2=GND, TP3=encoder A, TP4=encoder B.

GPIOs are 3.3 V only. Confirm OLED pin order from its module documentation before assembly. No battery charger, actuator, or exposed field wiring is present.
