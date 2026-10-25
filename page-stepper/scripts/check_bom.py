#!/usr/bin/env python3
"""Validate Page Stepper BOM columns and print the estimated parts total."""
import argparse
import csv
from decimal import Decimal, InvalidOperation
from pathlib import Path

EXPECTED = {"Ref", "Qty", "Value", "MPN", "Manufacturer", "Package", "Supplier", "UnitPrice", "Notes"}

def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv_file", nargs="?", type=Path,
                        default=Path(__file__).resolve().parents[1] / "hardware" / "BOM.csv")
    args = parser.parse_args()
    with args.csv_file.open(newline="", encoding="utf-8") as stream:
        reader = csv.DictReader(stream)
        if set(reader.fieldnames or ()) != EXPECTED:
            parser.error("BOM header does not match expected columns")
        refs, total = set(), Decimal("0")
        for line, row in enumerate(reader, start=2):
            ref = row["Ref"]
            if not ref or ref in refs:
                parser.error(f"line {line}: missing or duplicate reference {ref!r}")
            refs.add(ref)
            try:
                qty, unit = int(row["Qty"]), Decimal(row["UnitPrice"])
            except (ValueError, InvalidOperation):
                parser.error(f"line {line}: invalid quantity or unit price")
            if qty < 1 or unit < 0:
                parser.error(f"line {line}: quantity must be positive and price nonnegative")
            total += qty * unit
    print(f"{len(refs)} BOM lines; estimated total ${total:.2f}")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
