#!/usr/bin/env python3
"""Generate a synthetic Trap Sage maintenance route log."""

from __future__ import annotations

import csv
import datetime as dt
import random
from pathlib import Path

ROWS = [
    ("BLDG-A-TRAP-07", 158.0, 93.0, 24.5, 17.5, 0.357, 2.31, 0.91, 0.28, 6.67, 71.8, "NORMAL_CYCLING"),
    ("BLDG-A-TRAP-11", 154.0, 142.0, 24.0, 18.1, 0.783, 1.16, 0.08, 1.00, 10.91, 34.9, "BLOW_THROUGH"),
    ("BLDG-B-TRAP-03", 141.0, 38.0, 23.2, 16.8, 0.038, 2.42, 0.69, 0.00, 3.87, 77.7, "COLD_BLOCKED"),
    ("BLDG-C-TRAP-19", 149.0, 119.0, 25.1, 15.9, 0.417, 0.98, 0.21, 0.62, 4.62, 58.7, "FLOODING"),
    ("BLDG-D-VALVE-02", 88.0, 84.0, 22.7, 13.4, 0.562, 1.82, 0.71, 0.48, 17.14, 49.6, "CAVITATING_VALVE"),
]

FIELDS = [
    "timestamp",
    "asset_id",
    "upstream_temp_c",
    "downstream_temp_c",
    "ambient_temp_c",
    "clamp_force_n",
    "ultrasonic_rms",
    "crest_factor",
    "burstiness",
    "active_fraction",
    "cycle_rate_cpm",
    "health_score",
    "classification",
]


def main() -> None:
    rng = random.Random(73)
    start = dt.datetime(2026, 8, 25, 8, 0, tzinfo=dt.timezone.utc)
    out_path = Path(__file__).resolve().parents[1] / "docs" / "sample-log.csv"

    with out_path.open("w", newline="") as fh:
        writer = csv.DictWriter(fh, fieldnames=FIELDS)
        writer.writeheader()
        for index, row in enumerate(ROWS):
            asset_id, up, down, amb, force, rms, crest, burst, active, cpm, health, cls = row
            writer.writerow(
                {
                    "timestamp": (start + dt.timedelta(seconds=index * 133)).isoformat().replace("+00:00", "Z"),
                    "asset_id": asset_id,
                    "upstream_temp_c": round(up + rng.uniform(-0.2, 0.2), 2),
                    "downstream_temp_c": round(down + rng.uniform(-0.2, 0.2), 2),
                    "ambient_temp_c": round(amb + rng.uniform(-0.1, 0.1), 2),
                    "clamp_force_n": round(force + rng.uniform(-0.3, 0.3), 2),
                    "ultrasonic_rms": round(rms + rng.uniform(-0.01, 0.01), 3),
                    "crest_factor": round(crest + rng.uniform(-0.03, 0.03), 2),
                    "burstiness": round(burst + rng.uniform(-0.03, 0.03), 2),
                    "active_fraction": round(max(0.0, min(1.0, active + rng.uniform(-0.03, 0.03))), 2),
                    "cycle_rate_cpm": round(max(0.0, cpm + rng.uniform(-0.3, 0.3)), 2),
                    "health_score": round(max(0.0, min(100.0, health + rng.uniform(-1.0, 1.0))), 1),
                    "classification": cls,
                }
            )

    print(f"wrote {out_path}")


if __name__ == "__main__":
    main()
