#!/usr/bin/env python3
"""Validate a Loom Mark CSV event stream from stdin or a file."""
import argparse
import csv
import sys


def validate(stream):
    prior = 0
    for row in csv.reader(stream):
        if len(row) != 2:
            raise ValueError(f"expected 2 columns, got {len(row)}: {row}")
        pick, ms = map(int, row)
        if pick != prior + 1:
            raise ValueError(f"nonconsecutive pick number: {pick}")
        if not 0 <= ms <= 0xFFFFFFFF:
            raise ValueError(f"uptime out of uint32 range: {ms}")
        prior = pick
    return prior


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("file", nargs="?", help="CSV file; stdin if omitted")
    args = parser.parse_args()
    try:
        if args.file:
            with open(args.file, encoding="utf-8", newline="") as f:
                count = validate(f)
        else:
            count = validate(sys.stdin)
        print(f"valid rows: {count}")
    except (OSError, ValueError) as exc:
        parser.error(str(exc))


if __name__ == "__main__":
    main()
