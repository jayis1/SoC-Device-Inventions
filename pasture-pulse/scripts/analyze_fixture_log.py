#!/usr/bin/env python3
from __future__ import annotations

import csv
from collections import Counter
from pathlib import Path

log_path = Path(__file__).resolve().parents[1] / "docs" / "sample-log.csv"

faults = Counter()
peak_sum = 0.0
count = 0

with log_path.open() as f:
    reader = csv.DictReader(f)
    for row in reader:
        faults[row["fault"]] += 1
        peak_sum += float(row["pulse_kv"])
        count += 1

avg_peak = peak_sum / count if count else 0.0
print(f"rows={count}")
print(f"avg_peak_kv={avg_peak:.2f}")
for fault, qty in faults.most_common():
    print(f"{fault},{qty}")
