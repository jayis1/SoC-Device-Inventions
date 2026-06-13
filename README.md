# SoC Device Inventions

A curated collection of novel System-on-Chip device designs — each invented from the ground up with full schematics, firmware, and documentation. A new device drops every 24 hours.

## Philosophy

Every device here is designed with these principles:

- **Bottom-up design** — schematic first, then firmware, then application layer
- **Real buildable hardware** — uses available SoCs, standard packages, off-the-shelf passives
- **Open everything** — KiCad schematics, C firmware, Verilog where applicable, full BOM
- **Purpose-driven** — each device solves a real problem or enables a new capability

## Devices

| # | Device | SoC | Description |
|---|--------|-----|-------------|
| 1 | Neuro Sense Puck | ESP32-C6 | Wearable multi-modal environment sensor with on-device ML inference |
| 2 | Canopy Listener | RP2040 | Solar-powered acoustic biodiversity monitor with LoRa uplink and GPS timestamping |

## Structure

Each device lives in its own subfolder:

```
<device-name>/
├── README.md            # Device overview, usage, pinout
├── schematic/            # KiCad project files
├── firmware/             # C source + build system
├── hardware/             # BOM, gerbers, placement
├── verilog/               # Custom RTL (if any)
└── docs/                  # Datasheets, app notes
```

## License

MIT — build it, sell it, improve it.

---

*Invented and maintained by [jayis1](https://github.com/jayis1). New device every 24h.*