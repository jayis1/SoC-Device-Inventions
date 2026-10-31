#!/usr/bin/env python3
"""Validate the Backlash Bench BOM and check the index link."""
import csv
from pathlib import Path
p = Path(__file__).parents[1] / "hardware" / "BOM.csv"
with p.open(newline="", encoding="utf-8") as f:
    rows = list(csv.reader(f))
assert rows and rows[0] == ["Designator", "Qty", "Description", "Manufacturer / part", "Package / footprint", "Notes"]
assert all(len(row) == len(rows[0]) for row in rows)
assert all(int(row[1]) > 0 for row in rows[1:])
assert len({d for row in rows[1:] for d in row[0].split()}) == sum(len(row[0].split()) for row in rows[1:])
readme = Path(__file__).parents[2] / "README.md"
assert "[Backlash Bench](backlash-bench/)" in readme.read_text(encoding="utf-8")
print(f"BOM OK: {len(rows)-1} rows; index link OK")
