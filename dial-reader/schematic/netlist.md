# Connectivity-level schematic record (not a KiCad schematic)

- U1 (Raspberry Pi Pico) VSYS: USB 5 V; 3V3_OUT -> U2 VCC and optical circuit supply.
- Pico GND -> common return.
- Pico GP26/ADC0 <- U2 SIG (74HC4067 common analog terminal).
- Pico GP2..GP5 -> U2 S0..S3 respectively; U2 /EN tied GND; unused channel pins left open.
- Each channel uses Q1,Q2,Q3,Q4,Q5,Q6,Q7,Q8,Q9,Q10,Q11,Q12,Q13,Q14,Q15,Q16 TEPT4400 with emitter to GND and collector pulled to 3V3 through R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,R13,R14,R15,R16 (10 kΩ), then routed to its mux input; LED1,LED2,LED3,LED4,LED5,LED6,LED7,LED8,LED9,LED10,LED11,LED12,LED13,LED14,LED15,LED16 illuminate the pointer/face and are each fed from 3V3 through R17,R18,R19,R20,R21,R22,R23,R24,R25,R26,R27,R28,R29,R30,R31,R32 (1 kΩ) to GND. Confirm polarity and optical contrast in prototype measurements.
- SW1 (ZERO) and SW2 (SPAN) momentary normally-open switches connect GP6 and GP7 respectively to GND; internal pullups enabled.
- GP0/GP1 and GND exposed at debug header J2 as 3.3 V UART only.

Part references correspond to BOM. This describes intended nets; no PCB layout or electrical-rule check is provided. Confirm phototransistor pinout from the exact purchased package before assembly.
