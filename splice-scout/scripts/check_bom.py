#!/usr/bin/env python3
"""Validate Splice Scout BOM shape and positive quantities."""
import csv
from pathlib import Path

bom = Path(__file__).resolve().parents[1] / "hardware" / "BOM.csv"
with bom.open(newline="", encoding="utf-8") as stream:
    rows = list(csv.DictReader(stream))
required = {"Item", "Description", "Manufacturer part number", "Package", "Qty", "Notes"}
if not rows or set(rows[0]) != required:
    raise SystemExit("invalid BOM header or empty BOM")
for row in rows:
    if not all(row[key].strip() for key in required):
        raise SystemExit(f"incomplete BOM row: {row}")
    if int(row["Qty"]) < 1:
        raise SystemExit(f"invalid quantity: {row}")
print(f"BOM OK: {len(rows)} line items")
