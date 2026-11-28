#!/usr/bin/env python3
"""Summarize Twist Index DATA rows from a CSV-like firmware capture."""
import csv
import sys


def main() -> int:
    reader = csv.reader(sys.stdin)
    rows = [row for row in reader if len(row) == 4 and row[0] == "DATA"]
    for row in rows:
        try:
            turns, length, rate = map(int, row[1:])
        except ValueError:
            continue
        if length <= 0 or turns < 0 or rate < 0:
            continue
        print(f"turns={turns} length_mm={length} turns_per_km={rate}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
