# Loom Mark reference netlist

This is an explicit wiring-level reference, not a native KiCad schematic or fabrication-ready PCB. Build on a Raspberry Pi Pico module; do not substitute a bare RP2040.

- U1 Raspberry Pi Pico: USB powers the board; use its 3V3 output for the low-current sensor circuit. Common GND. GPIO2=GATE_A_N, GPIO3=GATE_B_N, GPIO25=onboard LED.
- D1/D2 TSAL6200 IR emitters: anode to 3V3 through R1/R2 (220 ohm), cathode to GND. Continuous emission; no switching transistors are fitted or needed.
- Q1/Q2 TEPT4400 phototransistors: collector to 3V3 through R3/R4 (100 kohm); emitter to GND. Each collector node connects to the corresponding comparator non-inverting input.
- U2 LMV393: VCC=3V3, GND=GND. For each channel, the inverting input receives the midpoint of a 100 kohm / 100 kohm divider (R7/R8 and R9/R10 respectively) between 3V3 and GND. Comparator outputs are open-collector; R5/R6 (10 kohm) pull them up to 3V3 and they connect to Pico GPIO2/GPIO3.
- C1 100 nF directly across U2 VCC/GND. C2 10 uF across 3V3/GND near the sensor connectors.
- J2/J3 each carry 3V3, GND, phototransistor collector signal, phototransistor emitter/GND. Key the connectors and label pin 1.
- J1 is the Pico's onboard USB connector. No external USB wiring.

The BOM must include the four threshold-divider resistors (R7-R10) in addition to R1-R6. Comparator threshold polarity/level and optical geometry need empirical adjustment; verify the output polarity before connecting the Pico. Keep GPIO inputs within 0-3.3 V. This design has not been physically validated.
