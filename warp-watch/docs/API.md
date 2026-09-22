# Warp Watch USB protocol

Designed and documented by jayis1.

The reference target exposes USB CDC at 115200 bit/s, 8-N-1. Frames are printable ASCII terminated by LF. Numeric tension is centinewtons (`cN`); angle is 0–4095 AS5600 counts. The firmware must reject an overlong line (>96 bytes), unknown command, bad field count, and out-of-range configuration without changing state.

Request / response:

```text
GET STATUS\n
STATUS,1,STABLE,425,1873,2\n
SET LIMITS,150,800,30\n
OK,1\n
CAL ZERO\n
OK,1\n
```

`STATUS,<version>,<state>,<tension_cN>,<angle_count>,<seconds_without_motion>` is version 1. States are `STABLE`, `SLACK`, `OVER_TENSION`, `TAKEUP_STALLED`, and `SENSOR_FAULT`. `SET LIMITS` accepts minimum cN, maximum cN, and motion timeout seconds. Valid ranges are minimum >= 5 cN, maximum > minimum and <= 5000 cN, timeout >= 5 seconds. `ERR,1,BAD_REQUEST` signals a parser or bounds failure; `ERR,1,SENSOR_FAULT` means output cues are inhibited until a valid sample returns.

No wireless, cloud, credentials, or user-identifying data are part of this design.
