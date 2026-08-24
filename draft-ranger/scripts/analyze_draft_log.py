#!/usr/bin/env python3
import csv
import statistics
import sys
from collections import Counter


def main(path: str) -> int:
    rows = []
    with open(path, newline="", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            row["pressure_pa"] = float(row["pressure_pa"])
            row["draft_velocity_mps"] = float(row["draft_velocity_mps"])
            row["condensation_margin_c"] = float(row["condensation_margin_c"])
            row["leak_severity"] = float(row["leak_severity"])
            rows.append(row)

    if not rows:
        print("no data")
        return 1

    worst = max(rows, key=lambda r: r["leak_severity"])
    classes = Counter(r["fault_class"] for r in rows)

    print(f"rows: {len(rows)}")
    print(f"avg pressure Pa: {statistics.mean(r['pressure_pa'] for r in rows):.2f}")
    print(f"avg velocity m/s: {statistics.mean(r['draft_velocity_mps'] for r in rows):.2f}")
    print(f"min condensation margin C: {min(r['condensation_margin_c'] for r in rows):.2f}")
    print(f"worst leak: mark={worst['mark']} severity={worst['leak_severity']:.1f} class={worst['fault_class']}")
    print("class histogram:")
    for key, value in classes.most_common():
        print(f"  {key}: {value}")
    return 0


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: analyze_draft_log.py <csv>")
        raise SystemExit(2)
    raise SystemExit(main(sys.argv[1]))
