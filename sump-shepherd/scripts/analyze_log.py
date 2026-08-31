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
    path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(__file__).with_name("demo_log.csv")
    rows = load_rows(path)
    if not rows:
        print("no rows")
        return 1

    counts = Counter(row["state"] for row in rows)
    max_level = max(float(row["level_mm"]) for row in rows)
    min_battery = min(float(row["battery_v"]) for row in rows)
    active_currents = [float(row["pump_current_a"]) for row in rows if float(row["pump_current_a"]) > 1.0]
    avg_active_current = statistics.mean(active_currents) if active_currents else 0.0

    print(f"rows: {len(rows)}")
    print(f"max_level_mm: {max_level:.1f}")
    print(f"min_battery_v: {min_battery:.2f}")
    print(f"avg_active_current_a: {avg_active_current:.2f}")
    print("state_counts:")
    for state, count in sorted(counts.items()):
        print(f"  {state}: {count}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
