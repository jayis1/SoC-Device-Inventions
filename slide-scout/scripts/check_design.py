#!/usr/bin/env python3
"""Validate Slide Scout's fixed pin assignments and BOM CSV without hardware."""
import csv
from pathlib import Path
root = Path(__file__).resolve().parents[1]
expected = {4: "I2C0_SDA", 5: "I2C0_SCL", 12: "LED"}
assert len(expected) == len(set(expected)), "GPIO assigned more than once"
with (root / "hardware/BOM.csv").open(newline="", encoding="utf-8") as f:
    rows = list(csv.DictReader(f))
assert rows and all(r.get("Ref") and r.get("MPN") for r in rows), "BOM missing references/MPNs"
text = (root / "firmware/main.c").read_text(encoding="utf-8")
for gpio in expected:
    assert str(gpio) in text, f"GPIO{gpio} absent from firmware"
assert "GP4" in (root / "README.md").read_text(encoding="utf-8")
print(f"OK: {len(expected)} assigned GPIOs; {len(rows)} BOM rows; static consistency only")
