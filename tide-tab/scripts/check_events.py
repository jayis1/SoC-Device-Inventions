#!/usr/bin/env python3
"""Validate Tide Tab JSONL records from stdin or a file; no hardware/network access."""
import json
import sys

source = open(sys.argv[1], encoding="utf-8") if len(sys.argv) > 1 else sys.stdin
last = -1
count = 0
for number, line in enumerate(source, 1):
    if not line.strip():
        continue
    try:
        row = json.loads(line)
        assert isinstance(row["ms"], int) and row["ms"] >= 0
        assert row["level_low"] in (True, False)
        assert row["ms"] >= last
    except (ValueError, KeyError, AssertionError, TypeError) as exc:
        raise SystemExit(f"line {number}: invalid record: {exc}")
    last = row["ms"]
    count += 1
print(f"valid records: {count}")
