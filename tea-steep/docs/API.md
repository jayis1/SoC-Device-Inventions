# USB CDC record contract

Header: `ms,temp_c,r,g,b,clear,status`

- `ms`: RP2040 uptime milliseconds
- `temp_c`: blank while temperature adapter is unavailable
- `r,g,b,clear`: TCS34725 unsigned 16-bit raw channel counts
- `status`: `OK` only if both temperature and color adapters report valid data; otherwise `ERR`

The initial firmware has no DS18B20 adapter, so it intentionally reports `ERR` even when color reads work. Counts are not calibrated units. No network service is present.
