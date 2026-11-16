#!/usr/bin/env python3
"""Validate and summarize Mate Trace CSV records; standard library only."""
import csv
import sys

FIELDS = ["run_id", "peak_counts", "sample_count"]

def main(path: str) -> int:
    with open(path, newline="", encoding="utf-8") as stream:
        reader = csv.DictReader(stream)
        if reader.fieldnames != FIELDS:
            raise ValueError(f"expected columns {FIELDS}")
        rows = list(reader)
    for line, row in enumerate(rows, 2):
        run_id, peak, count = (int(row[key]) for key in FIELDS)
        if run_id <= 0 or peak < 0 or count <= 0:
            raise ValueError(f"line {line}: invalid run values")
    print(f"valid_runs={len(rows)}")
    for row in rows:
        print(f"run={row['run_id']} peak_counts={row['peak_counts']} samples={row['sample_count']}")
    return 0

if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit(f"usage: {sys.argv[0]} measurements.csv")
    try:
        raise SystemExit(main(sys.argv[1]))
    except (OSError, ValueError) as exc:
        raise SystemExit(str(exc))
