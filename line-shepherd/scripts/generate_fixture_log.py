#!/usr/bin/env python3
"""Generate a synthetic Line Shepherd fixture log."""

from __future__ import annotations

import csv
import random
from datetime import datetime, timedelta, timezone
from pathlib import Path

FAULTS = [
    "OPEN_END",
    "SHORT_END",
    "HIGH_RESISTANCE_SPLICE",
    "WATER_INGRESS",
    "BRIDGE_TAP",
    "SPLIT_PAIR",
    "HEALTHY_LINE",
]


def gen_row(i: int, start: datetime) -> dict[str, object]:
    fault = random.choice(FAULTS)
    vf = random.choice([0.52, 0.57, 0.63, 0.68, 0.69, 0.80])
    dist = round(random.uniform(6.0, 120.0), 1)
    rt_ns = round((2.0 * dist) / (299792458.0 * vf) * 1e9, 1)
    peak = round(random.uniform(0.02, 0.85), 2)
    trough = round(-random.uniform(0.02, 0.70), 2)
    balance = round(random.uniform(60.0, 99.0), 1)
    health = round(max(0.0, min(100.0, 100.0 - random.uniform(5.0, 70.0))), 1)
    return {
        "timestamp": (start + timedelta(minutes=i)).isoformat().replace("+00:00", "Z"),
        "asset_id": f"FIXTURE-{i:03d}",
        "pair": random.choice(["PAIR_A", "PAIR_B", "PAIR_C", "PAIR_D"]),
        "cable_type": random.choice(["CAT5E", "CAT6", "ALARM_2C", "ALARM_4C", "IRRIGATION_2C"]),
        "velocity_factor": vf,
        "round_trip_ns": rt_ns,
        "est_distance_m": dist,
        "refl_peak_v": peak,
        "refl_trough_v": trough,
        "ringing_ratio": round(random.uniform(0.03, 0.45), 2),
        "loop_res_ohm": round(random.uniform(8.0, 100.0), 1),
        "cap_nf": round(random.uniform(1.0, 9.5), 1),
        "pair_balance_pct": balance,
        "health_score": health,
        "classification": fault,
    }


def main() -> int:
    out_path = Path(__file__).resolve().parents[1] / "docs" / "fixture-log.csv"
    start = datetime(2026, 8, 27, tzinfo=timezone.utc)
    rows = [gen_row(i, start) for i in range(12)]
    with out_path.open("w", newline="") as fh:
        writer = csv.DictWriter(fh, fieldnames=list(rows[0].keys()))
        writer.writeheader()
        writer.writerows(rows)
    print(out_path)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
