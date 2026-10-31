#!/usr/bin/env python3
"""Convert Thread Gauge sample CSV fields to textile linear-density units."""
import argparse
import csv
import sys


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--mass-mg", type=int, help="sample mass in milligrams")
    parser.add_argument("--length-mm", type=int, help="measured specimen length in millimetres")
    args = parser.parse_args()
    if args.mass_mg is None and args.length_mm is None:
        writer = csv.writer(sys.stdout)
        writer.writerow(["mass_mg", "length_mm", "tex", "denier"])
        return 0
    if args.mass_mg is None or args.length_mm is None or args.mass_mg <= 0 or args.length_mm <= 0:
        parser.error("both --mass-mg and --length-mm must be positive")
    tex = (args.mass_mg * 1000 + args.length_mm // 2) // args.length_mm
    denier = (args.mass_mg * 9000 + args.length_mm // 2) // args.length_mm
    print(f"mass_mg={args.mass_mg},length_mm={args.length_mm},tex={tex},denier={denier}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
