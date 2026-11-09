#!/usr/bin/env python3
"""Lightweight CSV/BOM and pin-map sanity checks; not ERC or electrical validation."""
import csv
from pathlib import Path
root = Path(__file__).resolve().parents[1]
with (root / "hardware/BOM.csv").open(newline="", encoding="utf-8") as f:
    rows = list(csv.DictReader(f))
assert rows and all(r["Ref"] and r["Qty"] and r["Description"] for r in rows)
refs = [r["Ref"] for r in rows]
assert len(refs) == len(set(refs)), "duplicate BOM reference designator"
net = (root / "schematic/netlist.md").read_text()
readme = (root / "README.md").read_text()
for token in ("GP4", "GP5", "0x29", "3V3"):
    assert token in net and token in readme, f"pin/rail mismatch: {token}"
assert "not a KiCad schematic" in net
assert (root / "firmware/src/main.c").is_file()
print(f"PASS: {len(rows)} BOM line items, required pin/rail strings agree; schematic explicitly identified as wiring reference")
