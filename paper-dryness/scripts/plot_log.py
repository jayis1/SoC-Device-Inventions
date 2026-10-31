#!/usr/bin/env python3
"""Summarize paper-dryness ADC CSV logs without modifying input."""
import argparse
import csv
import statistics
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv_file", type=Path, help="firmware CSV log")
    args = parser.parse_args()
    with args.csv_file.open(newline="", encoding="utf-8") as stream:
        rows = [int(row["adc_raw"]) for row in csv.DictReader(stream)]
    if not rows:
        parser.error("CSV contains no samples")
    print(f"samples={len(rows)} mean={statistics.mean(rows):.2f} "
          f"min={min(rows)} max={max(rows)} stdev={statistics.pstdev(rows):.2f}")


if __name__ == "__main__":
    main()
