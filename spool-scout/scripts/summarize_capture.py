#!/usr/bin/env python3
"""Validate and summarize Spool Scout JSONL records from a saved capture."""
import argparse
import json
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("capture", type=Path, help="UTF-8 JSONL capture file")
    args = parser.parse_args()
    count = faults = 0
    with args.capture.open(encoding="utf-8") as stream:
        for line_no, line in enumerate(stream, 1):
            if not line.strip():
                continue
            try:
                item = json.loads(line)
                if item.get("v") != 1 or item.get("state") not in {"ok", "sensor_fault", "motion_disagreement"}:
                    raise ValueError("unsupported version/state")
                count += 1
                faults += item["state"] != "ok"
            except (json.JSONDecodeError, KeyError, TypeError, ValueError) as exc:
                parser.error(f"line {line_no}: invalid record: {exc}")
    print(f"records={count} alerts={faults}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
