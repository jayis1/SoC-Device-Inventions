#!/usr/bin/env python3
"""Validate and summarize Knot Witness CDC CSV logs (stdlib only)."""
import argparse
import csv
import math
import sys

HEADER = ["time_ms", "raw", "force_n", "peak_n"]

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv_file", help="captured CDC CSV file")
    args = parser.parse_args()
    try:
        with open(args.csv_file, newline="", encoding="utf-8") as f:
            reader = csv.reader(f)
            header = next(reader, None)
            if header != HEADER:
                raise ValueError(f"expected header {HEADER!r}, got {header!r}")
            samples = 0
            last_time = -1
            peak = None
            for line, row in enumerate(reader, 2):
                if len(row) != 4:
                    raise ValueError(f"line {line}: expected 4 fields")
                t = int(row[0]); int(row[1])
                if t < last_time:
                    raise ValueError(f"line {line}: time moved backwards")
                last_time = t
                if row[2] != "NA":
                    force, reported_peak = float(row[2]), float(row[3])
                    if not math.isfinite(force) or not math.isfinite(reported_peak) or force < 0:
                        raise ValueError(f"line {line}: invalid force value")
                    peak = max(peak or 0.0, force)
                samples += 1
    except (OSError, ValueError, StopIteration) as exc:
        print(f"invalid log: {exc}", file=sys.stderr)
        return 2
    peak_text = "NA" if peak is None else f"{peak:.3f}"
    print(f"samples={samples} duration_ms={max(last_time, 0)} observed_peak_n={peak_text}")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
