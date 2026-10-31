#!/usr/bin/env python3
"""Decode Backlash Bench CSV rows from stdin or a named capture file."""
import argparse
import csv
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("file", nargs="?", help="CSV capture; defaults to stdin")
    args = parser.parse_args()
    source = open(args.file, newline="", encoding="utf-8") if args.file else sys.stdin
    with source:
        rows = csv.DictReader(source)
        required = {"elapsed_us", "input_deg", "output_deg", "direction"}
        if not required.issubset(rows.fieldnames or []):
            parser.error("input must contain elapsed_us,input_deg,output_deg,direction")
        for row in rows:
            try:
                elapsed = int(row["elapsed_us"])
                input_angle = float(row["input_deg"])
                output_angle = float(row["output_deg"])
                direction = int(row["direction"])
            except ValueError:
                continue  # Firmware ERROR rows are intentionally ignored.
            if direction not in (0, 1):
                continue
            print(f"{elapsed},{input_angle:.3f},{output_angle:.3f},{direction}")


if __name__ == "__main__":
    main()
