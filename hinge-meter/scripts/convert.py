#!/usr/bin/env python3
"""Convert Hinge Meter CSV raw ADC readings to force using a calibration factor."""
import argparse
import csv
import sys


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv_file", help="serial capture CSV (ms,angle_deg,raw_adc,force_n)")
    parser.add_argument("--counts-per-newton", type=float, required=True, help="calibrated raw counts per newton")
    args = parser.parse_args()
    if args.counts_per_newton == 0:
        parser.error("counts-per-newton must be nonzero")
    writer = csv.writer(sys.stdout)
    writer.writerow(("ms", "angle_deg", "raw_adc", "force_n"))
    with open(args.csv_file, newline="", encoding="utf-8") as stream:
        for row in csv.DictReader(line for line in stream if not line.startswith("#")):
            raw = int(row["raw_adc"])
            writer.writerow((row["ms"], row["angle_deg"], raw, f"{raw / args.counts_per_newton:.6f}"))
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
