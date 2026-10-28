#!/usr/bin/env python3
"""Summarize Darkroom Dose CSV rows from USB serial capture."""
import argparse
import csv
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv_file", type=Path, help="CSV containing exposure_ms,dose_counts rows")
    args = parser.parse_args()
    count = 0
    with args.csv_file.open(newline="", encoding="utf-8") as stream:
        for row in csv.reader(stream):
            if len(row) == 4 and row[0] == "exposure_ms" and row[2] == "dose_counts":
                int(row[1]); int(row[3]); count += 1
    print(f"valid exposure records: {count}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
