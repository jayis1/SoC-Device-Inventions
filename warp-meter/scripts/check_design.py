#!/usr/bin/env python3
"""Sanity-check the documented Warp Meter wiring and BOM references."""
import csv
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
EXPECTED = {"ENC_A": "GP2", "ENC_B": "GP3", "YARN_GATE": "GP14", "ZERO_BUTTON": "GP15", "LED_PIN": "GP25"}


def main() -> int:
    source = (ROOT / "firmware/main.c").read_text(encoding="utf-8")
    for symbol, pin in EXPECTED.items():
        if f"#define {symbol} {pin[2:]}" not in source:
            raise SystemExit(f"firmware pin mismatch: {symbol} should be {pin}")
    with (ROOT / "hardware/BOM.csv").open(newline="", encoding="utf-8") as handle:
        rows = list(csv.DictReader(handle))
    refs = {row["Ref"] for row in rows}
    required = {"U1", "ENC1", "S1", "R1", "SW1", "J1"}
    if refs != required:
        raise SystemExit(f"BOM reference mismatch; missing={required-refs}, extra={refs-required}")
    if any(not row["MPN"].strip() or not row["Manufacturer"].strip() for row in rows):
        raise SystemExit("BOM row missing manufacturer or MPN")
    print("pin definitions and BOM references: OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
