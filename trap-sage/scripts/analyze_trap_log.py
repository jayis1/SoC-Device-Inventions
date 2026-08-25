#!/usr/bin/env python3
"""Summarize a Trap Sage CSV route log."""

from __future__ import annotations

import csv
import statistics
import sys
from collections import Counter, defaultdict
from pathlib import Path


def main(path: str) -> int:
    rows = []
    with Path(path).open() as fh:
        reader = csv.DictReader(fh)
        for row in reader:
            row["health_score"] = float(row["health_score"])
            row["ultrasonic_rms"] = float(row["ultrasonic_rms"])
            rows.append(row)

    if not rows:
        print("no rows")
        return 1

    counts = Counter(row["classification"] for row in rows)
    by_class = defaultdict(list)
    for row in rows:
        by_class[row["classification"]].append(row["health_score"])

    print(f"rows={len(rows)}")
    print("class_counts=")
    for name, count in counts.most_common():
        mean_health = statistics.fmean(by_class[name])
        print(f"  {name}: count={count} mean_health={mean_health:.1f}")

    worst = min(rows, key=lambda row: row["health_score"])
    print(
        f"worst_asset={worst['asset_id']} classification={worst['classification']} health={worst['health_score']:.1f}"
    )
    return 0


if __name__ == "__main__":
    target = sys.argv[1] if len(sys.argv) > 1 else str(Path(__file__).resolve().parents[1] / "docs" / "sample-log.csv")
    raise SystemExit(main(target))
