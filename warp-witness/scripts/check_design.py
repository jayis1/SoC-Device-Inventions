#!/usr/bin/env python3
"""Offline consistency checks for Warp Witness files; no hardware or network access."""
import csv
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
readme = (ROOT / "README.md").read_text()
net = (ROOT / "schematic/netlist.md").read_text()
with (ROOT / "hardware/BOM.csv").open(newline="") as f:
    rows = list(csv.DictReader(f))
assert len(rows) == 12, f"expected 12 BOM rows, got {len(rows)}"
for gpio in ("GP2", "GP3", "GP4", "GP6", "GP7", "GP8", "GP10", "GP11", "GP14", "GP15"):
    assert gpio in readme and gpio in net, f"pin missing from README/netlist: {gpio}"
assert "not a KiCad schematic" in net
print(f"OK: {len(rows)} BOM lines; 10 GPIO assignments cross-checked")
