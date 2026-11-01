#!/usr/bin/env python3
"""Validate Braille Check CSV records from an adapter or hand-prepared test."""
import argparse
import csv
import math

FIELDS = ["dot", "base_um", "top_um", "height_um"]

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv_file")
    args = parser.parse_args()
    with open(args.csv_file, newline="", encoding="utf-8") as stream:
        reader = csv.DictReader(stream)
        if reader.fieldnames != FIELDS:
            parser.error("expected columns: " + ",".join(FIELDS))
        count = 0
        for line, row in enumerate(reader, start=2):
            try:
                dot = int(row["dot"])
                vals = [float(row[k]) for k in FIELDS[1:]]
            except (ValueError, TypeError):
                parser.error(f"line {line}: invalid numeric field")
            if dot not in range(6) or not all(math.isfinite(v) for v in vals):
                parser.error(f"line {line}: dot must be 0..5 and values finite")
            if not math.isclose(vals[2], vals[1] - vals[0], abs_tol=0.5):
                parser.error(f"line {line}: height does not match top-base")
            count += 1
        if count != 6:
            parser.error(f"expected six dot records, got {count}")
    print("valid: six internally consistent records")

if __name__ == "__main__":
    main()
