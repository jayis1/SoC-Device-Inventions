#!/usr/bin/env python3
"""Convert Pollen Pixel CSV exports into a simple daily exposure summary."""

from __future__ import annotations

import argparse
import csv
from collections import defaultdict


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("csv_file", help="CSV exported by the Pollen Pixel firmware")
    return p.parse_args()


def main() -> None:
    args = parse_args()
    daily = defaultdict(lambda: {"samples": 0, "grains_m3": 0.0, "allergen": 0.0})

    with open(args.csv_file, newline="", encoding="utf-8") as f:
        for row in csv.DictReader(f):
            day = row["timestamp"].split("T", 1)[0]
            daily[day]["samples"] += 1
            daily[day]["grains_m3"] += float(row["grains_m3"])
            daily[day]["allergen"] += float(row["allergen_index"])

    print("date,samples,avg_grains_m3,avg_allergen_index")
    for day in sorted(daily):
        samples = daily[day]["samples"]
        avg_g = daily[day]["grains_m3"] / samples
        avg_a = daily[day]["allergen"] / samples
        print(f"{day},{samples},{avg_g:.1f},{avg_a:.1f}")


if __name__ == "__main__":
    main()
