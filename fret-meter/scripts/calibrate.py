#!/usr/bin/env python3
"""Compute linear calibration endpoints from raw ADC and measured mm values."""
import argparse

p = argparse.ArgumentParser()
p.add_argument("raw0", type=int); p.add_argument("mm0", type=float)
p.add_argument("raw1", type=int); p.add_argument("mm1", type=float)
a = p.parse_args()
if not (0 <= a.raw0 <= 4095 and 0 <= a.raw1 <= 4095) or a.raw0 == a.raw1:
    p.error("raw endpoints must be distinct values in 0..4095")
slope = (a.mm1 - a.mm0) / (a.raw1 - a.raw0)
print(f"mm = {slope:.9g} * (raw - {a.raw0}) + {a.mm0:.9g}")
