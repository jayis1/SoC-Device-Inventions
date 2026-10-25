#!/usr/bin/env python3
"""Small static consistency checks for the Dial Reader reference design."""
import csv
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parents[1]
expected = ["Ref", "Qty", "Value", "MPN", "Manufacturer", "Package", "Notes"]
with (ROOT / "hardware/BOM.csv").open(newline="", encoding="utf-8") as stream:
    reader = csv.DictReader(stream)
    if reader.fieldnames != expected:
        raise SystemExit(f"unexpected BOM header: {reader.fieldnames}")
    rows = list(reader)
if not rows or any(not all(row.values()) for row in rows):
    raise SystemExit("empty BOM or incomplete row")
netlist = (ROOT / "schematic/netlist.md").read_text(encoding="utf-8")
for row in rows:
    # Ranges are represented in the connectivity description as R1..R16, etc.
    first = re.match(r"([A-Z]+)(\d+)(?:-([A-Z]*)(\d+))?$", row["Ref"])
    if not first:
        raise SystemExit(f"unsupported reference range: {row['Ref']}")
    start, end = int(first.group(2)), int(first.group(4) or first.group(2))
    prefix = first.group(3) or first.group(1)
    for number in range(start, end + 1):
        ref = f"{prefix}{number}"
        if ref not in netlist:
            raise SystemExit(f"BOM reference absent from netlist: {ref}")
config = (ROOT / "firmware/include/board_config.h").read_text(encoding="utf-8")
firmware = (ROOT / "firmware/src/main.c").read_text(encoding="utf-8")
pins = re.findall(r"#define PIN_\w+\s+(\d+)u", config)
if len(pins) != len(set(pins)):
    raise SystemExit("duplicate GPIO assignment in board_config.h")
for token in re.findall(r"#define (PIN_\w+)", config):
    if token not in firmware:
        raise SystemExit(f"configured pin unused in firmware: {token}")
print(f"PASS: {len(rows)} BOM rows; references present in netlist; {len(pins)} unique GPIO assignments used by firmware")

# Design documentation and measurements are credited to the repository owner.
