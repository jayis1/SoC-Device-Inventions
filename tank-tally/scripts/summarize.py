#!/usr/bin/env python3
"""Summarize Tank Tally event CSV lines from stdin or a file."""
import argparse
import csv
import sys

def main():
    p = argparse.ArgumentParser()
    p.add_argument("file", nargs="?", help="log file; defaults to stdin")
    args = p.parse_args()
    stream = open(args.file, newline="", encoding="utf-8") if args.file else sys.stdin
    rows = list(csv.reader(stream))
    events = []
    for row in rows:
        if row and row[0] == "event":
            if len(row) != 4:
                raise SystemExit(f"invalid event row: {row!r}")
            events.append((int(row[1]), int(row[2]), int(row[3])))
    print(f"events={len(events)}")
    for idx, elapsed, interval in events:
        print(f"{idx},{elapsed},{interval}")

if __name__ == "__main__":
    main()
