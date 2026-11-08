#!/usr/bin/env python3
"""Plot Clay Ledger CSV captured from USB CDC. Requires matplotlib."""
import argparse
import csv
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv_file", type=Path)
    args = parser.parse_args()
    if not args.csv_file.is_file():
        parser.error(f"not a file: {args.csv_file}")
    times, positions = [], []
    with args.csv_file.open(newline="", encoding="utf-8") as stream:
        rows = (line for line in stream if not line.startswith("#"))
        for row in csv.DictReader(rows):
            try:
                times.append(float(row["t_ms"]) / 60000.0)
                positions.append(float(row["position_mm"]))
            except (KeyError, ValueError, TypeError):
                continue
    if not times:
        parser.error("no valid position rows found")
    import matplotlib.pyplot as plt
    plt.plot(times, positions, marker=".")
    plt.xlabel("Elapsed time (min)")
    plt.ylabel("Relative carriage displacement (mm)")
    plt.title("Clay Ledger drying test")
    plt.grid(True)
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
