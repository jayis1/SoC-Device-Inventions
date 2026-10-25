#!/usr/bin/env python3
"""Validate and summarize Dial Reader USB CDC JSONL records."""
import json
import sys


def main() -> int:
    count = 0
    for line_no, line in enumerate(sys.stdin, 1):
        try:
            row = json.loads(line)
        except json.JSONDecodeError as exc:
            print(f"line {line_no}: invalid JSON: {exc}", file=sys.stderr)
            return 2
        if not isinstance(row, dict) or row.get("status") not in {"reading", "uncalibrated"}:
            print(f"line {line_no}: unexpected record shape", file=sys.stderr)
            return 2
        if "angle_deg" not in row:
            print(f"line {line_no}: missing angle_deg", file=sys.stderr)
            return 2
        count += 1
    print(f"validated {count} records")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
