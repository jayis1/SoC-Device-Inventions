#!/usr/bin/env python3
"""Summarize Bounce Bench transition CSV from stdin or a file; no hardware access."""
import argparse
import csv
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("file", nargs="?", help="CSV file (default: stdin)")
    args = parser.parse_args()
    stream = open(args.file, newline="", encoding="utf-8") if args.file else sys.stdin
    rows = list(csv.reader(stream))
    changes = [r for r in rows if len(r) == 2 and r[0].replace(".", "", 1).isdigit()]
    print(f"transitions={len(changes)}")
    if changes:
        print(f"first_ms={changes[0][0]} last_ms={changes[-1][0]}")


if __name__ == "__main__":
    main()
