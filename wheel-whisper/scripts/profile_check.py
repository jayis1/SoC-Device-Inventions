#!/usr/bin/env python3
"""Check a Wheel Whisper wheel-profile CSV. Designed by jayis1.

CSV columns: spoke_id,frequency_hz,target_hz. This tool evaluates relative
frequency spread; it is not an absolute tension calibration or safety decision.
"""
import csv
import sys
from statistics import median


def main(path: str) -> int:
    values = []
    with open(path, newline="", encoding="utf-8") as handle:
        for row in csv.DictReader(handle):
            try:
                hz = float(row["frequency_hz"])
                target = float(row["target_hz"])
            except (KeyError, ValueError) as exc:
                print(f"invalid row: {row} ({exc})", file=sys.stderr)
                return 2
            if hz <= 0 or target <= 0:
                print(f"non-positive frequency: {row}", file=sys.stderr)
                return 2
            values.append((row.get("spoke_id", "?"), hz, target))
    if len(values) < 3:
        print("need at least three spoke measurements", file=sys.stderr)
        return 2
    measured = [item[1] for item in values]
    center = median(measured)
    spread = (max(measured) - min(measured)) / center * 100.0
    print(f"count={len(values)} median_hz={center:.2f} range_percent={spread:.2f}")
    for spoke, hz, target in values:
        delta = (hz - target) / target * 100.0
        print(f"{spoke}: {hz:.2f} Hz, target delta {delta:+.2f}%")
    return 0


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: profile_check.py measurements.csv", file=sys.stderr)
        raise SystemExit(2)
    raise SystemExit(main(sys.argv[1]))
