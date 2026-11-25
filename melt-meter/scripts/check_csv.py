#!/usr/bin/env python3
"""Validate Melt Meter CSV shape and status/temperature consistency."""
import csv
import sys
from io import StringIO

SAMPLE = "elapsed_ms,temp_c,status\n0,24.00,OK\n250,,FAULT\n"

def validate(stream):
    reader = csv.DictReader(stream)
    if reader.fieldnames != ["elapsed_ms", "temp_c", "status"]:
        raise ValueError("unexpected CSV header")
    count = 0
    previous = -1
    for row in reader:
        count += 1
        elapsed = int(row["elapsed_ms"])
        if elapsed < previous:
            raise ValueError("timestamps must be nondecreasing")
        previous = elapsed
        if row["status"] == "OK":
            float(row["temp_c"])
        elif row["status"] == "FAULT":
            if row["temp_c"] != "":
                raise ValueError("fault row must not contain a temperature")
        else:
            raise ValueError("status must be OK or FAULT")
    return count

if __name__ == "__main__":
    try:
        n = validate(StringIO(SAMPLE)) if len(sys.argv) == 1 else validate(open(sys.argv[1], newline="", encoding="utf-8"))
    except (OSError, ValueError, TypeError) as exc:
        print(f"invalid: {exc}", file=sys.stderr)
        raise SystemExit(1)
    print(f"valid rows: {n}")
