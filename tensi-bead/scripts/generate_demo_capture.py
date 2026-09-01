#!/usr/bin/env python3
import csv
from pathlib import Path

ROWS = [
    {"sample_id": 1, "mode": "pendant", "surface_tension_mn_m": 62.23, "contact_angle_deg": "", "classification": "high-surface-tension / water-like"},
    {"sample_id": 2, "mode": "pendant", "surface_tension_mn_m": 56.84, "contact_angle_deg": "", "classification": "high-surface-tension / water-like"},
    {"sample_id": 3, "mode": "pendant", "surface_tension_mn_m": 43.99, "contact_angle_deg": "", "classification": "medium-surface-tension / solvent-blend"},
    {"sample_id": 4, "mode": "pendant", "surface_tension_mn_m": 20.40, "contact_angle_deg": "", "classification": "low-surface-tension / surfactant-or-solvent-rich"},
    {"sample_id": 5, "mode": "sessile", "surface_tension_mn_m": 72.18, "contact_angle_deg": 39.0, "classification": "high-energy / easily wetted surface"},
    {"sample_id": 6, "mode": "sessile", "surface_tension_mn_m": 64.08, "contact_angle_deg": 84.0, "classification": "moderately wetting surface"},
    {"sample_id": 7, "mode": "sessile", "surface_tension_mn_m": 68.76, "contact_angle_deg": 58.0, "classification": "high-energy / easily wetted surface"},
    {"sample_id": 8, "mode": "pendant", "surface_tension_mn_m": 72.84, "contact_angle_deg": "", "classification": "high-surface-tension / water-like"},
]


def main() -> int:
    out = Path(__file__).with_name("demo_capture.csv")
    with out.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=list(ROWS[0].keys()))
        writer.writeheader()
        writer.writerows(ROWS)
    print(out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
