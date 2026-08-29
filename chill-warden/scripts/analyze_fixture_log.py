#!/usr/bin/env python3
from __future__ import annotations

import csv
import sys
from collections import Counter
from pathlib import Path


def load_rows(path: Path):
    with path.open(newline="") as handle:
        return list(csv.DictReader(handle))


def main() -> int:
    path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(__file__).resolve().parents[1] / "docs" / "sample-log.csv"
    rows = load_rows(path)
    if not rows:
        print("no rows found")
        return 1

    counts = Counter(row["fault"] for row in rows)
    max_risk = max(float(row["risk_score"]) for row in rows)
    mean_risk = sum(float(row["risk_score"]) for row in rows) / len(rows)
    hottest = max(rows, key=lambda row: float(row["discharge_c"]))
    noisiest = max(rows, key=lambda row: float(row["acoustic_db"]))

    print(f"rows={len(rows)} mean_risk={mean_risk:.1f} max_risk={max_risk:.1f}")
    print("fault_counts=" + ", ".join(f"{fault}:{counts[fault]}" for fault in sorted(counts)))
    print(f"highest_discharge_asset={hottest['asset_id']} discharge_c={hottest['discharge_c']} fault={hottest['fault']}")
    print(f"noisiest_asset={noisiest['asset_id']} acoustic_db={noisiest['acoustic_db']} fault={noisiest['fault']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
