#!/usr/bin/env python3
"""Generate a quick concentration lookup chart for field calibration."""

from __future__ import annotations

import math

FLOW_LPM = 1.8
EFFICIENCY = 0.86
PATCH_MM2 = 0.92
TAPE_MM2 = 14.0

print("count,seconds,grains_m3")
for count in (1, 2, 5, 10, 20, 40):
    for seconds in (60, 120, 300, 600):
        sampled_volume = (FLOW_LPM / 1000 / 60) * seconds
        grains = (count * (TAPE_MM2 / PATCH_MM2)) / (sampled_volume * EFFICIENCY)
        print(f"{count},{seconds},{grains:.1f}")
