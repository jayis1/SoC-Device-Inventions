#!/usr/bin/env python3
"""Check the Row Beacon BOM CSV structure using Python's standard library."""
import argparse
import csv
from pathlib import Path

EXPECTED = ["Ref", "Qty", "Value", "Package", "MPN", "Manufacturer", "Distributor", "DPN", "Notes"]

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("bom", nargs="?", default=str(Path(__file__).resolve().parents[1] / "hardware" / "BOM.csv"))
    args = parser.parse_args()
    with open(args.bom, newline="", encoding="utf-8") as f:
        rows = list(csv.reader(f))
    if not rows or rows[0] != EXPECTED:
        raise SystemExit("BOM header mismatch")
    if any(len(row) != len(EXPECTED) for row in rows[1:]):
        raise SystemExit("BOM row has wrong column count")
    for n, row in enumerate(rows[1:], start=2):
        try:
            if int(row[1]) <= 0:
                raise ValueError
        except ValueError:
            raise SystemExit(f"invalid Qty at CSV line {n}")
        if not row[4] or not row[5]:
            raise SystemExit(f"missing MPN/manufacturer at CSV line {n}")
    print(f"OK: {len(rows)-1} BOM rows")

if __name__ == "__main__":
    main()
