#!/usr/bin/env python3
"""Summarize a Line Shepherd CSV log."""

from __future__ import annotations

import csv
import statistics
import sys
from collections import Counter
from pathlib import Path


def main(path: str) -> int:
    rows = []
    with Path(path).open() as fh:
        reader = csv.DictReader(fh)
        for row in reader:
            row["est_distance_m"] = float(row["est_distance_m"])
            row["health_score"] = float(row["health_score"])
            rows.append(row)

    if not rows:
        print("no rows")
        return 1

    counts = Counter(row["classification"] for row in rows)
    print(f"rows={len(rows)}")
    print(f"mean_distance_m={statistics.fmean(row['est_distance_m'] for row in rows):.2f}")
    print(f"mean_health={statistics.fmean(row['health_score'] for row in rows):.2f}")
    print("class_counts=")
    for name, count in counts.most_common():
        print(f"  {name}: {count}")

    worst = min(rows, key=lambda row: row["health_score"])
    print(
        f"worst_asset={worst['asset_id']} fault={worst['classification']} distance_m={worst['est_distance_m']:.1f} health={worst['health_score']:.1f}"
    )
    return 0


if __name__ == "__main__":
    target = sys.argv[1] if len(sys.argv) > 1 else str(Path(__file__).resolve().parents[1] / "docs" / "sample-log.csv")
    raise SystemExit(main(target))
