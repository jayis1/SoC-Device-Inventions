#!/usr/bin/env python3
"""Convert a trace_end JSONL record from Leaf Ledger to calibrated mm²."""
import argparse
import json
import sys

parser = argparse.ArgumentParser()
parser.add_argument("x_counts_per_mm", type=float)
parser.add_argument("y_counts_per_mm", type=float)
args = parser.parse_args()
if args.x_counts_per_mm <= 0 or args.y_counts_per_mm <= 0:
    parser.error("calibration scales must be positive")
for line in sys.stdin:
    try:
        item = json.loads(line)
    except json.JSONDecodeError:
        continue
    if item.get("event") == "trace_end":
        area = item["area_counts2"] / (args.x_counts_per_mm * args.y_counts_per_mm)
        print(json.dumps({"event": "area", "area_mm2": area, "points": item["points"]}))
