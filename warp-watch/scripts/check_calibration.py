#!/usr/bin/env python3
"""Validate a two-point Warp Watch calibration capture CSV."""
import argparse
import csv
import sys


def read_rows(path):
    with open(path, newline="", encoding="utf-8") as handle:
        rows = list(csv.DictReader(handle))
    required = {"known_cN", "raw_count"}
    if not rows or set(rows[0]) != required:
        raise ValueError("CSV must have exactly known_cN,raw_count columns")
    return [(float(row["known_cN"]), float(row["raw_count"])) for row in rows]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("capture", help="CSV with known_cN,raw_count")
    args = parser.parse_args()
    points = read_rows(args.capture)
    if len(points) < 2:
        raise ValueError("at least two calibration points are required")
    low, high = points[0], points[-1]
    denominator = high[1] - low[1]
    if denominator == 0:
        raise ValueError("raw counts must differ")
    slope = (high[0] - low[0]) / denominator
    intercept = low[0] - slope * low[1]
    print(f"slope_cN_per_count={slope:.9f}")
    print(f"intercept_cN={intercept:.3f}")
    return 0

if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, csv.Error) as error:
        print(f"error: {error}", file=sys.stderr)
        raise SystemExit(2)
