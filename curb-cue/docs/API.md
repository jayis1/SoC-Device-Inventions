# Curb Cue configuration and status reference

Designed and documented by jayis1. The reference firmware has no cloud API and sends no telemetry. BLE configuration is optional and must require a local user action (mode-button hold) before advertising.

## Local policy interface

```text
input:  low_mm, high_mm, low_valid, high_valid
config: near_mm (200..3999), caution_mm (near_mm+1..4000)
output: state, left_motor, right_motor, pulse_ms
```

| State | Meaning | Haptic output |
|---|---|---|
| `CLEAR` | both valid returns exceed caution threshold | off |
| `CAUTION` | one return at or below caution threshold | 45 ms pulse on matching side |
| `NEAR` | one return at or below near threshold | 90 ms pulse on matching side |
| `BOTH_NEAR` | both returns at or below near threshold | 120 ms simultaneous pulse |
| `SENSOR_FAULT` | invalid/stale return or invalid configuration | 80 ms simultaneous diagnostic pulse; inhibit normal cues |

## Optional BLE configuration service

Use a private, authenticated GATT service chosen during product implementation; do not reuse this document as a Bluetooth SIG-assigned UUID specification. Characteristics should be writable only during local configuration mode:

| Field | Encoding | Safe default |
|---|---|---|
| `near_mm` | uint16 little-endian | 750 |
| `caution_mm` | uint16 little-endian | 1500 |
| `scan_rate_hz` | uint8 | 10 |
| `enabled` | uint8, 0/1 | 1 |

Reject `near_mm < 200`, `caution_mm <= near_mm`, `caution_mm > 4000`, scan rates outside 1–15 Hz, and any write when local configuration mode is inactive. Store settings atomically with wear limits; if storage is corrupt, restore documented defaults and signal `SENSOR_FAULT` once.
