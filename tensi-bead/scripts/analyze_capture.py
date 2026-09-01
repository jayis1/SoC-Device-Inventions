#!/usr/bin/env python3
import csv
import statistics
import sys
from collections import Counter
from pathlib import Path


def load_rows(path: Path):
    with path.open(newline="") as f:
        return list(csv.DictReader(f))


def main() -> int:
    path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(__file__).with_name("demo_capture.csv")
    rows = load_rows(path)
    if not rows:
        print("no rows")
        return 1

    gammas = [float(r["surface_tension_mn_m"]) for r in rows]
    angles = [float(r["contact_angle_deg"]) for r in rows if r["contact_angle_deg"]]
    modes = Counter(r["mode"] for r in rows)
    labels = Counter(r["classification"] for r in rows)

    print(f"rows: {len(rows)}")
    print(f"gamma_mean_mn_m: {statistics.mean(gammas):.2f}")
    print(f"gamma_min_mn_m: {min(gammas):.2f}")
    print(f"gamma_max_mn_m: {max(gammas):.2f}")
    print(f"angle_mean_deg: {statistics.mean(angles):.2f}" if angles else "angle_mean_deg: n/a")
    print("mode_counts:")
    for key, value in sorted(modes.items()):
        print(f"  {key}: {value}")
    print("classification_counts:")
    for key, value in sorted(labels.items()):
        print(f"  {key}: {value}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
