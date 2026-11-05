#!/usr/bin/env python3
"""Summarize Wick Watch CSV events from stdin or a file."""
import argparse
import csv
import sys


def summarize(rows):
    times = {}
    timed_out = False
    for row in rows:
        try:
            event, sensor, elapsed = row["event"], int(row["sensor"]), int(row["elapsed_ms"])
        except (KeyError, ValueError, TypeError):
            continue
        if event == "arrival" and 1 <= sensor <= 5:
            times[sensor] = elapsed
        elif event == "timeout":
            timed_out = True
    return times, timed_out


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv_file", nargs="?", help="CSV path (default: stdin)")
    args = parser.parse_args()
    stream = open(args.csv_file, newline="", encoding="utf-8") if args.csv_file else sys.stdin
    try:
        times, timed_out = summarize(csv.DictReader(stream))
    finally:
        if args.csv_file:
            stream.close()
    print("sensor,elapsed_ms")
    for sensor in sorted(times):
        print(f"{sensor},{times[sensor]}")
    if timed_out:
        print("# run timed out", file=sys.stderr)


if __name__ == "__main__":
    main()
