#!/usr/bin/env python3
from __future__ import annotations

import csv
from datetime import datetime, timedelta, timezone
from pathlib import Path

rows = [
    ("north", 7.4, 2.1, 1.03, 46, 3.33, "FENCE_HEALTHY"),
    ("south", 5.1, 4.6, 1.02, 51, 3.30, "VEGETATION_LOADING"),
    ("east", 4.9, 2.8, 1.01, 97, 3.27, "WET_INSULATOR_LEAKAGE"),
    ("west", 1.2, 0.2, 1.02, 62, 3.28, "OPEN_GATE_OR_BREAK"),
    ("remote", 3.6, 1.7, 1.09, 44, 2.92, "WEAK_ENERGIZER_BATTERY"),
    ("ridge", 9.8, 0.7, 0.27, 88, 3.24, "SURGE_DISTURBANCE"),
    ("far", 0.0, 0.0, 4.80, 59, 3.22, "FENCE_DOWN"),
]

start = datetime(2026, 8, 30, 6, 0, tzinfo=timezone.utc)
out_path = Path(__file__).resolve().parents[1] / "docs" / "sample-log.csv"

with out_path.open("w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["timestamp", "segment", "pulse_kv", "current_a", "interval_s", "humidity_rh", "battery_v", "fault"])
    for index, row in enumerate(rows):
        ts = start + timedelta(seconds=10 * index)
        writer.writerow([ts.isoformat().replace("+00:00", "Z"), *row])

print(f"wrote {out_path}")
