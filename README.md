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
|| 1 | Neuro Sense Puck | ESP32-C6 | Wearable multi-modal environment sensor with on-device ML inference |
|| 2 | Canopy Listener | RP2040 | Solar-powered acoustic biodiversity monitor with LoRa uplink and GPS timestamping |
|| 3 | Brew Sense | STM32L476RG | Precision fermentation monitor with vibrating-tube densitometer, CO₂, pH, and BLE/Wi-Fi uplink |
|| 4 | Scribe Nib | ESP32-S3 | Pen-clip smart handwriting digitizer with 9-axis IMU, on-device CNN recognition, and BLE HID keyboard |
|| 5 | Flux Ring | nRF52840 | Finger-worn magnetic field explorer with haptic + LED feedback, tilt-compensated compass, and BLE spatial mapping |
|| 6 | Tremor Tile | RP2040 | Solar-powered structural vibration monitor with ultra-low-noise accelerometer, on-device FFT analysis, anomaly detection, and LoRa uplink |
|| 7 | Hive Mind | STM32WL55JC | Solar-powered beehive health monitor with 50kg load cell, 3× DS18B20 temperature probes, I2S acoustic classifier, IR bee counter, and LoRaWAN uplink |
|| 8 | Echo Mote | ESP32-S3 | Pocket-sized room acoustic analyzer with stereo MEMS mic array, swept-sine speaker output, on-device FFT, RT60/clarity/room-mode detection, and BLE/Wi-Fi reporting |
|| 9 | Soil Whisper | STM32WL55JC | Solar-powered agricultural soil intelligence probe with multi-depth moisture, NPK ion-selective electrodes, pH, temperature, and LoRaWAN uplink |
|| 10 | Mycelium Node | ESP32-C6-MINI-1 | Mushroom fruiting chamber environmental controller with multi-zone temp/humidity/CO₂ sensing, PID-controlled actuators, growth phase scheduling, and WiFi/BLE remote monitoring |
|| 11 | Melody Sprite | RP2040 | Handheld 8-voice FM synthesizer with capacitive touch keyboard, real-time pot control, I2S audio output, OLED display, step sequencer, and BLE MIDI |
|| 12 | Therma Weave | ESP32-C3 | Multi-zone heated textile controller with PID control, 8-channel NTC thermistor monitoring, activity-adaptive heating, BLE smartphone control, OLED display, and triple-redundant safety watchdog |
|| 13 | Spectra Charm | STM32G491RET6 + ESP32-C3 | Pocket UV-Vis spectrophotometer with AS7343 10-channel sensor, broadband+UV LED sweep, 128-point spectral deconvolution, 200-compound library matching, Beer-Lambert quantification, and BLE/Wi-Fi reporting |
|| 14 | Phase Scope | STM32G491RET6 | Handheld 3-phase power quality analyzer with galvanically-isolated voltage/current inputs, on-device FFT harmonic analysis, transient capture, phasor diagram OLED, BLE streaming, and SD card logging |
| 15 | Ping Caliper | STM32G474RET6 + ESP32-C3 | Pocket ultrasonic thickness gauge & flaw detector with HV GaN pulser, TGC receiver, 5 Msps A-scan capture, 60+ material velocity database, echo-to-echo through-coating mode, SD logging, and BLE/Wi-Fi streaming to a phone app |
| 16 | Echo Trap | ESP32-S3-WROOM-1 | Solar-powered acoustic insect trap with dual ICS-43434 MEMS mics, on-device int8 1D-CNN wingbeat species classifier (12 classes), UV LED lure, suction fan capture, LoRaWAN uplink, and field deployment for precision agriculture pest monitoring |
| 17 | Sap Watch | STM32WL55JC | Solar-powered trunk-mounted sap-flow sensor using the heat-ratio method with 3-needle probe, 24-bit ADS122U04 ADC, wounding-corrected sap-flux computation, drought-stress anomaly detection, and LoRaWAN uplink for forestry research and irrigation scheduling |
| 18 | Pulse Hound | ESP32-S3-WROOM-1 | Pocket RF signal hunter with AD8318 1 MHz–8 GHz log detector, 28BYJ-48 motorized direction-finding antenna, geiger-counter audio feedback, OLED spectrum waterfall, signal classification, BLE streaming, and SD card logging for bug sweeping and RF interference hunting |
| 19 | Mussel Watch | nRF52840 + SX1262 | Submersible bivalve biomonitoring sensor that clips onto a living mussel, measures valve-gape angle via DRV5053 Hall sensor + ADS1115 ADC, combines with DS18B20 water temp, Atlas DO dissolved oxygen, MS5837 depth, anomaly detection (closure events, rhythm deviation, multi-mussel stress), and LoRaWAN uplink for biological early-warning water-quality monitoring |
| 20 | Aero Reed | ESP32-S3-WROOM-1 | Pocket-sized breath-controlled electronic wind instrument (EWI) with 14 capacitive-touch saxophone fingering pads, dual MP3V5004G breath + FSR-402 lip/bite pressure sensing, ICM-42688-P IMU tilt expression, 16-voice wavetable synth with bore resonator, I2S PCM5102A stereo audio, MAX98357A speaker, OLED display, 8 NVS-stored patches, and simultaneous BLE + USB MIDI |
| 21 | Ferro Weave | STM32G474RET6 + ESP32-C3 | Pocket magnetic hysteresis loop tracer (B-H curve analyzer) that drives a programmable ±2 A current sweep through a specimen winding, integrates the secondary voltage for flux B, samples current for field H, and computes B_sat/H_c/B_r/μ/core-loss/squareness with CORDIC-accelerated math, OLED loop plot, SD logging, and BLE/Wi-Fi streaming to a PC plotting app |
| 22 | Sky Lens | ESP32-S3-WROOM-1 | Pocket cosmic-ray muon telescope with dual plastic-scintillator + SiPM coincidence detection, 14-bit pulse-height capture, track zenith-angle reconstruction, IMU-fused celestial muon skymap, pressure-corrected flux, optional muon-lifetime (τ_µ ≈ 2.2 µs) measurement mode, OLED dashboard, SD event logging, and BLE/Wi-Fi live event streaming |
| 23 | Gravi Dot | STM32G474RET6 + ESP32-C3 | Handheld portable relative gravimeter for micro-gal subsurface anomaly mapping using ADXL355 ultra-low-noise MEMS accelerometer, Peltier-stabilized copper thermal block, SCL3300 tilt compensation, MS5837 pressure correction, NEO-M9N GPS station tagging, Longman earth-tide model, base-loop drift correction, on-device Bouguer/free-air/latitude correction chain, SD survey logging, and BLE/Wi-Fi streaming for cave/void detection, archaeological prospection, and mineral exploration |
| 24 | Terra Pin | ESP32-S3-WROOM-1 | Handheld soil microbiome activity probe with dual Sensirion SCD41 NDIR CO₂ sensors for in-situ soil respiration flux measurement, Atlas Scientific EZO-ORP redox potential + EZO-EC conductivity electrodes, capacitive moisture probe, DS18B20 soil temperature, Q₁₀-corrected Soil Health Index (0–100) fusion model, PTFE membrane gas chamber, OLED display, SD logging, and BLE streaming for regenerative agriculture, carbon farming verification, compost maturity testing, and soil ecology research |
|| 25 | Spiro Flow | CH32V203RBT6 (RISC-V) + ESP32-C3 | Pocket-sized electronic spirometer with Fleisch pneumotachograph, Sensirion SDP810 differential pressure sensor at 250 Hz, BTPS correction via BME280, on-device FVC/FEV1/PEF/FEF25-75 computation, ATS/ERS 2019 quality grading, ECSC/ERS 1993 predicted-value comparison, diagnostic pattern classification, coaching buzzer, OLED flow-volume loop display, W25Q128 session logging, and BLE/WiFi relay via ESP32-C3 for affordable pulmonary function screening |
| 28 | Bolt Compass | ESP32-S3-WROOM-1 + ADS131M04 | Solar-powered pocket lightning direction-finder and storm-warning station with crossed shielded-loop VLF magnetic antennas (3–30 kHz sferics), chopper-stabilized slow E-field electrometer, 4-channel 24-bit simultaneous-sampling ADC, GPS PPS microsecond time-tagging, on-device CFAR detection + int8 CG/IC/CC stroke classification, Earth-ionosphere waveguide distance estimation, crossed-loop goniometer bearing, DBSCAN storm-cell tracker with approach alerts, OLED polar radar display, SD waveform logging, and BLE/Wi-Fi streaming |
|| 26 | Quartz Tuner | STM32G491RET6 | Handheld crystal oscillator parameter analyzer with Si5351A stimulus + AD5933 impedance receiver, π-network transmission method, IEC 444 motional parameter extraction (R₁/C₁/L₁/C₀/Q/ESR), admittance circle fitting, temperature turnover characterization with resistive heater + DS18B20, Allan deviation measurement, crystal cut classification (AT/BT/XY-fork/SC), ZIF fixture for HC-49/SMD packages, OLED display, SD logging, and BLE streaming for oscillator design and crystal QC |
|| 27 | Volt Scribe | STM32G491RET6 | Pocket-sized 3-electrode potentiostat/galvanostat for field electrochemistry with AD8606 control amplifier + OPA196 TIA, 7-decade auto-ranging (1 nA–10 mA), ADS1115 16-bit ADC, techniques including CV, DPV, SWV, EIS (1 Hz–100 kHz), and amperometry, Randles circuit fitting, peak detection, OLED voltammogram/Nyquist plot, SD logging, and BLE streaming for water quality testing, heavy metal detection, and biosensor readout |
| 29 | Frost Point | STM32L476RG | Pocket chilled-mirror dew point hygrometer with novel differential-thermistor film detection (no optical alignment), TEC1-12706 peltier mirror, ADS122U04 24-bit ADC, PID loop holding equilibrium film temperature, IR-scatter frost-vs-dew phase discrimination, BME280/SCD41/SHT45/MS5837 aux sensors, BLE + SD logging, bringing the NIST-traceable primary humidity method down to ~$120 and coffee-mug size |
| 30 | Lumen Cast | STM32G491RET6 + ESP32-C3 | Pocket 2-axis goniophotometer that sweeps a precision photometric head (TI OPT3001 lux + AMS TCS34725 RGB) around a light source on a NEMA8 stepper (azimuth) + SG90 servo (elevation), computing luminous flux by spherical integration, beam angle (FWHM), center-beam candlepower, CCT/Duv uniformity, isocandela plots, and exporting IES LM-63 / EULUMDAT photometric files via BLE/Wi-Fi — bringing $10k+ lab goniophotometry down to ~$77 and shoebox size |

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