#!/usr/bin/env python3
"""Summarize Tape Tally JSONL event records from stdin or a file."""
import argparse
import json
from pathlib import Path

parser = argparse.ArgumentParser()
parser.add_argument("log", type=Path)
args = parser.parse_args()
pulses = stalls = 0
for number, line in enumerate(args.log.read_text(encoding="utf-8").splitlines(), 1):
    if not line.strip():
        continue
    try:
        event = json.loads(line)
    except json.JSONDecodeError as exc:
        raise SystemExit(f"line {number}: invalid JSON: {exc}") from exc
    if event.get("event") == "pulse":
        pulses += 1
    elif event.get("event") == "stall":
        stalls += 1
print(f"pulses={pulses} stalls={stalls}")
