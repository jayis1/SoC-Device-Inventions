#!/usr/bin/env python3
import csv
import sys
from collections import Counter
from pathlib import Path


def main() -> int:
    if len(sys.argv) != 2:
        print("usage: analyze_fluorescence_log.py <csv>")
        return 1

    path = Path(sys.argv[1])
    rows = list(csv.DictReader(path.open()))
    if not rows:
        print("no rows")
        return 1

    avg_fvfm = sum(float(r["fv_fm"]) for r in rows) / len(rows)
    avg_phi = sum(float(r["phi_psii"]) for r in rows) / len(rows)
    worst = max(rows, key=lambda r: float(r["stress_index"]))
    counts = Counter(r["diagnosis"] for r in rows)

    print(f"samples: {len(rows)}")
    print(f"average Fv/Fm: {avg_fvfm:.3f}")
    print(f"average PhiPSII: {avg_phi:.3f}")
    print(f"worst leaf: {worst['leaf_id']} stress_index={float(worst['stress_index']):.1f} diagnosis={worst['diagnosis']}")
    print("diagnosis histogram:")
    for name, count in sorted(counts.items()):
        print(f"  {name}: {count}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
