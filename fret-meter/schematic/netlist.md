Reference wiring for the proposed prototype. This is deliberately not represented as a KiCad file; verify footprints and mechanical design before fabrication.

```text
J1 Raspberry Pi Pico
  36 (3V3 OUT) ---- RV1 pin 1 (10k linear)
  33 (AGND) ------- RV1 pin 3
  33 (AGND) ------- C1 100nF ---+--- GND
  RV1 pin 2 ------- R1 1k ------+--- J1 pin 31 GP26/ADC0
  J1 pin 19 GP14 --- SW1 NO -------- GND (firmware pull-up)
  USB connector ---- host USB 5V (Pico onboard regulation)
```

Pico pin references are physical header numbers; confirm the purchased Pico board's revision and silkscreen. Keep ADC wiring short and route away from USB edges. No external voltage above 3.3 V is permitted on ADC/GPIO.
