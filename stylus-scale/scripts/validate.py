#!/usr/bin/env python3
"""Validate the Stylus Scale deliverable's local files and BOM structure."""
import csv
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
rows = list(csv.DictReader((ROOT / "hardware/BOM.csv").open(newline="")))
assert rows and all(row["Ref"] and row["Qty"] and row["MPN"] for row in rows)
readme = ROOT / "README.md"
links = re.findall(r"\]\(([^)]+)\)", readme.read_text())
missing = [link for link in links if not (readme.parent / link).exists()]
assert not missing, f"unresolved README links: {missing}"
required = ["README.md", "schematic/netlist.md", "firmware/CMakeLists.txt", "firmware/src/stylus_scale.c", "hardware/BOM.csv", "docs/ASSEMBLY.md", "docs/API.md", "scripts/calibrate.py"]
assert all((ROOT / item).is_file() and (ROOT / item).stat().st_size for item in required)
print(f"valid: {len(rows)} BOM rows, {len(links)} README links, {len(required)} required files")
