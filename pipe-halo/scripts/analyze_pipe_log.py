#!/usr/bin/env python3
"""Analyze Pipe Halo CSV logs and print a compact risk summary."""

from __future__ import annotations

import csv
import sys
from collections import Counter


def main(path: str) -> int:
    counts: Counter[str] = Counter()
    max_burst = 0.0
    coldest_margin = 999.0

    with open(path, newline="", encoding="utf-8") as fh:
        reader = csv.DictReader(fh)
        for row in reader:
            state = row.get("state", "UNKNOWN")
            counts[state] += 1
            max_burst = max(max_burst, float(row.get("burst_risk_score", 0.0)))
            coldest_margin = min(coldest_margin, float(row.get("freeze_margin_c", 999.0)))

    print("Pipe Halo log summary")
    print(f"samples={sum(counts.values())}")
    for state, count in counts.most_common():
        print(f"{state}={count}")
    print(f"max_burst_risk={max_burst:.3f}")
    print(f"min_freeze_margin={coldest_margin:.3f}")
    return 0


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: analyze_pipe_log.py <csv>")
        raise SystemExit(2)
    raise SystemExit(main(sys.argv[1]))
