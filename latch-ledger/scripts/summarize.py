#!/usr/bin/env python3
"""Summarize Latch Ledger USB CDC JSON-lines logs."""
import argparse
import json
import statistics
import sys


def summarize(stream):
    cycles = []
    pending_open = None
    malformed = 0
    for line in stream:
        try:
            item = json.loads(line)
            if item.get("event") == "open":
                pending_open = int(item["ms"])
            elif item.get("event") == "close" and pending_open is not None:
                dwell = int(item.get("dwell_ms", int(item["ms"]) - pending_open))
                if dwell < 0:
                    raise ValueError("negative dwell")
                cycles.append(dwell)
                pending_open = None
        except (ValueError, TypeError, KeyError, json.JSONDecodeError):
            malformed += 1
    return cycles, malformed


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("file", nargs="?", help="JSONL log (default: stdin)")
    args = parser.parse_args()
    try:
        source = open(args.file, encoding="utf-8") if args.file else sys.stdin
        with source if args.file else _nullcontext(source):
            cycles, malformed = summarize(source)
    except OSError as exc:
        parser.error(str(exc))
    print(json.dumps({"complete_cycles": len(cycles), "dwell_ms": cycles,
                      "mean_dwell_ms": statistics.mean(cycles) if cycles else None,
                      "malformed_lines": malformed}))


class _nullcontext:
    def __init__(self, value): self.value = value
    def __enter__(self): return self.value
    def __exit__(self, *args): return False


if __name__ == "__main__":
    main()
