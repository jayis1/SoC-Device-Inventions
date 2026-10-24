#!/usr/bin/env python3
"""Decode Loom Latch JSONL events from stdin and print verified pick totals."""
import json
import sys


def main() -> int:
    count = 0
    for number, line in enumerate(sys.stdin, 1):
        try:
            event = json.loads(line)
        except json.JSONDecodeError as exc:
            print(f"line {number}: invalid JSON: {exc}", file=sys.stderr)
            return 2
        if event.get("event") != "pick" or event.get("direction") not in {"A>B", "B>A"}:
            print(f"line {number}: unexpected event", file=sys.stderr)
            return 2
        duration = event.get("duration_us")
        if not isinstance(duration, int) or not 300 <= duration <= 500000:
            print(f"line {number}: invalid duration", file=sys.stderr)
            return 2
        count += 1
    print(f"verified_picks={count}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
