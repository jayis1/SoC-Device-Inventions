# Cargo Canary event and provisioning API

Cargo Canary stores a fixed 24-byte little-endian record. It is intentionally compact enough for flash append and cellular batching.

| Offset | Bytes | Field |
|---:|---:|---|
| 0 | 2 | ASCII `CC` magic |
| 2 | 1 | format version (1) |
| 3 | 1 | flags |
| 4 | 4 | sequence number |
| 8 | 4 | estimated UTC seconds (0 when unknown) |
| 12 | 4 | monotonic seconds since activation |
| 16 | 2 | signed temperature, centi-degrees C |
| 18 | 2 | shock peak, mg |
| 20 | 2 | battery, mV |
| 22 | 2 | CRC-16/CCITT-FALSE across bytes 0–21 |

## Flags

| Bit | Name | Meaning |
|---:|---|---|
| 0 | TEMP_EXCURSION | out-of-limit persistence threshold met |
| 1 | TEMP_RECOVERED | recovery persistence threshold met |
| 2 | SHOCK | configured shock trigger occurred |
| 3 | TAMPER | normally-closed reed loop opened |
| 4 | SUSPECT_OPEN | light increase without proof of reed opening |
| 5 | LOCATION_UNAVAILABLE | GNSS attempt elapsed without a valid fix |
| 6 | LOW_BATTERY | modem disabled to protect local logging |

## BLE provisioning

BLE is for a trusted commissioning tool only. It must require authenticated pairing and encrypted link before it permits range, sample interval, APN, server endpoint, or erase changes. Never permit unauthenticated erase or an arbitrary TLS trust anchor over BLE.

## Cellular batch contract

A batch includes device serial, firmware version, record count, and complete byte records. The server must return an authenticated receipt containing each accepted `sequence` and the matching record CRC. The device records that receipt separately and does not erase records merely because a TCP/TLS request completed. Replay is safe because the server deduplicates on device serial + sequence + CRC.

Designed and documented by jayis1.
