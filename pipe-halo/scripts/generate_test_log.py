#!/usr/bin/env python3
"""Generate a small synthetic Pipe Halo CSV log for dashboard testing."""

from __future__ import annotations

import csv
import sys

ROWS = [
    ["2026-08-22T00:00:00Z", "IDLE", 0.052, 0.018, 0.000, 20.5, 0.020],
    ["2026-08-22T00:05:00Z", "NORMAL_FLOW", 0.451, 0.141, 0.000, 15.8, 0.110],
    ["2026-08-22T00:10:00Z", "DRIP_LEAK", 0.302, 0.124, 0.910, 6.1, 0.211],
    ["2026-08-22T00:15:00Z", "WATER_HAMMER", 0.688, 0.808, 0.000, 11.2, 0.357],
    ["2026-08-22T00:20:00Z", "FREEZE_RISK", 0.076, 0.043, 0.000, 0.3, 0.508],
    ["2026-08-22T00:25:00Z", "BURST_RISK", 0.612, 0.903, 0.000, -0.4, 0.815],
]


def main(path: str) -> int:
    with open(path, "w", newline="", encoding="utf-8") as fh:
        writer = csv.writer(fh)
        writer.writerow([
            "timestamp",
            "state",
            "flow_index",
            "hammer_index",
            "drip_confidence",
            "freeze_margin_c",
            "burst_risk_score",
        ])
        writer.writerows(ROWS)
    print(f"wrote {len(ROWS)} rows to {path}")
    return 0


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: generate_test_log.py <out.csv>")
        raise SystemExit(2)
    raise SystemExit(main(sys.argv[1]))
