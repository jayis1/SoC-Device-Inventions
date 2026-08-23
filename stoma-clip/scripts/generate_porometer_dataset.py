#!/usr/bin/env python3
"""Generate a synthetic Stoma Clip field dataset."""

from __future__ import annotations

import csv
import math
from datetime import datetime, timedelta
from pathlib import Path

OUT = Path(__file__).resolve().parents[1] / "docs" / "sample-log.csv"


def row(i: int, t: datetime) -> list[str]:
    leaf_temp = 28.5 + 1.2 * math.sin(i / 5)
    air_temp = 30.2 + 0.8 * math.sin(i / 7)
    air_rh = 41.0 + 6.5 * math.cos(i / 6)
    vpd = 2.1 + 0.35 * math.sin(i / 9)
    gsw = 0.28 - 0.006 * i + 0.02 * math.sin(i / 4)
    gsw = max(gsw, 0.05)
    transp = gsw * vpd * 0.0168
    stress = min(100.0, max(0.0, 18 + i * 2.6 - 6 * math.sin(i / 3)))
    condition = (
        "hydrated" if stress < 35 else
        "watch" if stress < 55 else
        "stressed" if stress < 80 else
        "severe"
    )
    fit_r2 = 0.992 + 0.006 * math.cos(i / 8)
    light = 14000 + 3200 * math.sin(i / 10)
    batt = 4.05 - 0.008 * i
    return [
        t.isoformat(),
        f"{leaf_temp:.2f}",
        f"{air_temp:.2f}",
        f"{air_rh:.2f}",
        f"{vpd:.3f}",
        f"{gsw:.3f}",
        f"{transp:.5f}",
        f"{stress:.1f}",
        condition,
        f"{fit_r2:.4f}",
        f"{light:.0f}",
        f"{batt:.2f}",
    ]


def main() -> None:
    start = datetime(2026, 8, 23, 10, 0, 0)
    OUT.parent.mkdir(parents=True, exist_ok=True)
    with OUT.open("w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow([
            "timestamp",
            "leaf_temp_c",
            "ambient_temp_c",
            "ambient_rh_pct",
            "vpd_kpa",
            "g_sw_mol_m2_s",
            "transpiration_g_m2_s",
            "stress_score",
            "condition",
            "fit_r2",
            "light_lux_proxy",
            "battery_v",
        ])
        for i in range(24):
            writer.writerow(row(i, start + timedelta(minutes=15 * i)))
    print(f"wrote {OUT}")


if __name__ == "__main__":
    main()
