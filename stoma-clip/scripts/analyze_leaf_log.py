#!/usr/bin/env python3
"""Analyze a Stoma Clip CSV log and print irrigation guidance."""

from __future__ import annotations

import csv
import statistics
import sys
from pathlib import Path


def main() -> int:
    path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(__file__).resolve().parents[1] / "docs" / "sample-log.csv"
    with path.open() as f:
        rows = list(csv.DictReader(f))

    if not rows:
        print("no rows")
        return 1

    gsw = [float(r["g_sw_mol_m2_s"]) for r in rows]
    stress = [float(r["stress_score"]) for r in rows]
    vpd = [float(r["vpd_kpa"]) for r in rows]

    mean_gsw = statistics.fmean(gsw)
    min_gsw = min(gsw)
    mean_stress = statistics.fmean(stress)
    max_stress = max(stress)
    mean_vpd = statistics.fmean(vpd)

    print(f"samples: {len(rows)}")
    print(f"mean stomatal conductance: {mean_gsw:.3f} mol m^-2 s^-1")
    print(f"minimum stomatal conductance: {min_gsw:.3f} mol m^-2 s^-1")
    print(f"mean stress score: {mean_stress:.1f}/100")
    print(f"peak stress score: {max_stress:.1f}/100")
    print(f"mean VPD: {mean_vpd:.2f} kPa")

    if max_stress >= 80 or min_gsw < 0.08:
        recommendation = "Irrigate immediately or reduce evaporative load."
    elif mean_stress >= 55 or mean_gsw < 0.14:
        recommendation = "Plan irrigation soon; plants show sustained stomatal restriction."
    elif mean_stress >= 35:
        recommendation = "Watch closely; stress emerging under current VPD."
    else:
        recommendation = "No immediate irrigation action indicated by the leaf dataset."

    print(f"recommendation: {recommendation}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
