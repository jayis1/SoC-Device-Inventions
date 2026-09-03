#!/usr/bin/env python3
"""Summarize Torque Pebble CSV logs."""

from __future__ import annotations

import csv
import sys
from collections import Counter


def main(path: str) -> int:
    counts: Counter[str] = Counter()
    max_peak = 0.0
    mean_angle = 0.0
    rows = 0

    with open(path, newline="", encoding="utf-8") as fh:
        reader = csv.DictReader(fh)
        for row in reader:
            rows += 1
            counts[row["state"]] += 1
            max_peak = max(max_peak, float(row["peak_torque_nm"]))
            mean_angle += float(row["angle_after_snug_deg"])

    mean_angle = mean_angle / rows if rows else 0.0

    print("Torque Pebble log summary")
    print(f"samples={rows}")
    for state, count in counts.most_common():
        print(f"{state}={count}")
    print(f"max_peak_torque_nm={max_peak:.2f}")
    print(f"mean_angle_after_snug_deg={mean_angle:.2f}")
    return 0


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: analyze_log.py <csv>")
        raise SystemExit(2)
    raise SystemExit(main(sys.argv[1]))
