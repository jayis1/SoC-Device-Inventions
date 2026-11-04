#!/usr/bin/env python3
"""Convert one raw capture row using explicit user calibration constants."""
import argparse
import csv
import sys

def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--force-counts-per-n", type=float, required=True)
    p.add_argument("--angle-counts-per-mm", type=float, required=True)
    p.add_argument("--zero-force", type=int, default=0)
    p.add_argument("--zero-angle", type=int, default=0)
    args = p.parse_args()
    if args.force_counts_per_n <= 0 or args.angle_counts_per_mm <= 0:
        p.error("calibration scales must be positive")
    rows = csv.reader(sys.stdin)
    writer = csv.writer(sys.stdout)
    writer.writerow(["millis", "force_n", "travel_mm"])
    for row in rows:
        if not row or row[0].startswith("#") or row[0] == "millis":
            continue
        if len(row) != 3:
            raise SystemExit("expected millis,raw_force,angle_raw")
        ms, force, angle = map(int, row)
        writer.writerow([ms, (force - args.zero_force) / args.force_counts_per_n, (angle - args.zero_angle) / args.angle_counts_per_mm])

if __name__ == "__main__":
    main()
