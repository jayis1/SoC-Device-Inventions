# Wheel Whisper local BLE and measurement API

Designed by jayis1. The target adapter is not implemented or validated in this reference tree.

## Measurement packet

The `Measurement` characteristic uses little-endian binary fields:

| Offset | Type | Field | Meaning |
|---:|---|---|---|
| 0 | u16 | sequence | monotonically increasing local measurement number |
| 2 | u8 | rim_index | optional route position, 0–31; 255 unknown |
| 3 | u8 | flags | bit 0 valid, bit 1 saturated, bit 2 weak, bit 3 ambiguous |
| 4 | u32 | frequency_cHz | fundamental frequency in centi-Hz |
| 8 | u16 | quality_centi | best/second spectral energy ratio × 100 |
| 10 | i16 | battery_mV | measured cell voltage, millivolts |

A host must reject any packet without `valid`. The device does not claim absolute tension in the packet because that requires a wheel-specific, independently validated profile.

## Commands

| Value | Command | Required behavior |
|---:|---|---|
| `0x01` | arm capture | next qualifying tap triggers one capture |
| `0x02` | clear route | erase local route samples only after physical confirmation |
| `0x03` | sleep | stop advertising after disconnect and enter low-power state |

Use BLE authenticated pairing/bonding supported by the chosen nRF52840 stack. Do not expose control writes to unauthenticated peers. The reference design stores no account credentials and has no cloud endpoint.
