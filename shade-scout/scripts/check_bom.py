#!/usr/bin/env python3
"""Check basic Shade Scout BOM structure without external dependencies."""
import csv
from pathlib import Path

p = Path(__file__).resolve().parents[1] / "hardware" / "BOM.csv"
with p.open(newline="", encoding="utf-8") as f:
    rows = list(csv.DictReader(f))
required = {"Ref", "Qty", "Part", "Notes"}
if not rows or set(rows[0]) != required:
    raise SystemExit("BOM is empty or has unexpected columns")
for row in rows:
    try:
        if int(row["Qty"]) < 1:
            raise ValueError
    except ValueError:
        raise SystemExit(f"invalid quantity for {row['Ref']}")
print(f"BOM OK: {len(rows)} line items")
