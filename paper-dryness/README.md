# Paper Dryness Comparator

A low-voltage bench instrument for comparing moisture-related dielectric response in paper sheets. Conservators, print shops, and book artists can compare a suspect sheet with a reference without pinning, heating, or otherwise damaging it. It reports a relative index only; it is not a calibrated moisture meter.

## Design status

Reference design, not physically validated. Sensor geometry and repeatability require characterization with controlled humidity samples. Use the Raspberry Pi Pico (RP2040) as the development module; no custom PCB is required for first bring-up.

## Blocks

```
USB 5 V -> Pico 3.3 V -> MCP3008 ADC <- guarded copper-plate RC probe
                       -> SSD1306 OLED (I2C)
                       -> USB CDC CSV
```

The probe is a low-voltage parallel-plate capacitor: place paper between two 50 x 50 mm copper electrodes separated by a fixed insulating frame. A 100 kΩ resistor charges the probe; firmware measures the discharge interval through a 1 MΩ series resistor into an MCP3008 input. This timing approach is an experimental concept; the included firmware currently provides a safe baseline serial instrument and does not claim a validated capacitance conversion.

## Pin assignment (Pico header GPIO)

| Function | Pico GPIO | MCP3008 / peripheral |
|---|---:|---|
| SPI0 RX | GP16 | MCP3008 DOUT |
| SPI0 CSn | GP17 | MCP3008 CS/SHDN |
| SPI0 SCK | GP18 | MCP3008 CLK |
| SPI0 TX | GP19 | MCP3008 DIN |
| I2C0 SDA | GP4 | OLED SDA |
| I2C0 SCL | GP5 | OLED SCL |
| Probe timing input | GP14 | RC comparator output (future revision) |
| Status LED | GP25 | Pico onboard LED |

3.3 V logic only. MCP3008 reference and supply are 3.3 V; probe electrodes are passive and never connected to an external source. Use Pico's USB supply only. Do not use for archival decisions without independent gravimetric calibration.

## BOM

See `hardware/BOM.csv`. Approximate cost assumes development-module assembly and excludes enclosure and shipping.

## Build and use

Requires Pico SDK 2.x and CMake. `cmake -S firmware -B build -DPICO_SDK_PATH=/path/to/pico-sdk && cmake --build build`. Flash `paper_dryness.uf2` by holding BOOTSEL while connecting USB. Open USB CDC at 115200 baud; firmware emits a CSV header and periodic `sample,adc_raw` readings. The raw ADC scaffold is included so the first prototype can validate the module and host data path before adding the characterized RC probe circuit.

## Limitations

No custom KiCad schematic or PCB is supplied: `schematic/netlist.md` documents the reference connections. This is a comparative research aid, not a calibrated instrument; paper type, electrode pressure, temperature, and surface coatings influence readings. No conservation, safety, or material-quality claims are implied.
