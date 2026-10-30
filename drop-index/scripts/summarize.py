#!/usr/bin/env python3
"""Summarize Drop Index CSV rows piped from a saved serial capture."""
import argparse
import csv
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv_file", nargs="?", help="capture file; default stdin")
    args = parser.parse_args()
    source = open(args.csv_file, newline="", encoding="utf-8") if args.csv_file else sys.stdin
    rates = []
    with source:
        reader = csv.DictReader(row for row in source if row.startswith("elapsed_s," ) or row[:1].isdigit())
        if reader.fieldnames != ["elapsed_s", "mass_g", "rate_g_per_min"]:
            raise SystemExit("expected header: elapsed_s,mass_g,rate_g_per_min")
        for row in reader:
            try:
                t, mass, rate = int(row["elapsed_s"]), float(row["mass_g"]), float(row["rate_g_per_min"])
            except (ValueError, TypeError):
                raise SystemExit("invalid numeric CSV row")
            if not 1 <= t <= 600 or not all(map(lambda x: abs(x) < 1e6, (mass, rate))):
                raise SystemExit("row outside accepted bounds")
            rates.append(rate)
    if not rates:
        raise SystemExit("no sample rows")
    print(f"samples={len(rates)} mean_rate_g_per_min={sum(rates)/len(rates):.3f} peak_rate_g_per_min={max(rates):.3f}")


if __name__ == "__main__":
    main()
