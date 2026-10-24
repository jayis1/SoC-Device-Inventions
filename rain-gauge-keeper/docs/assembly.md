# Assembly notes

1. Verify the microSD breakout accepts 3.3 V supply and logic. Do not use a 5 V-only module without level shifting; this reference design assumes native 3.3 V SPI.
2. Connect Pico GPIO2 to one reed contact and GND to the other. The switch is normally open; the internal plus external pull-up keeps the input high until a tip.
3. Wire SPI exactly as in the device pin table. Keep card leads short (preferably under 10 cm); add local 100 nF and 10 µF bypass capacitors.
4. Power from USB only. Before outdoor deployment, use a sealed enclosure with cable glands and drip loops; the current design has no ingress or surge rating.
5. With USB connected, confirm idle output, then manually close the reed input. Confirm one `TIP,<uptime_ms>` event per closure. Repeat rapidly to verify the 40 ms debounce.
6. Compare known water volume with bucket-tip count to establish the gauge-specific mm/tip factor. Repeat several pours; do not assume a universal bucket calibration.

No PCB, enclosure, or field validation is supplied. Do not attach long exposed cable during thunderstorms.
