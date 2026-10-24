#!/usr/bin/env python3
"""Capture Stitch Sentinel USB serial CSV to a file (stdlib only)."""
import argparse
import csv
import sys
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("port", help="serial device, e.g. /dev/ttyACM0")
    parser.add_argument("output", help="output CSV path")
    args = parser.parse_args()
    try:
        with open(args.port, "rb", buffering=0) as source, open(args.output, "w", newline="", encoding="utf-8") as dest:
            writer = csv.writer(dest)
            writer.writerow(["host_time", "ms", "event", "cycles", "feeds", "alarm"])
            while True:
                line = source.readline().decode("ascii", errors="replace").strip()
                fields = next(csv.reader([line]))
                if len(fields) != 5 or fields[0] == "ms":
                    continue
                try:
                    int(fields[0]); int(fields[2]); int(fields[3]); alarm = int(fields[4])
                    if alarm not in (0, 1):
                        continue
                except ValueError:
                    continue
                writer.writerow([time.time(), *fields])
                dest.flush()
    except KeyboardInterrupt:
        return 0
    except OSError as exc:
        print(f"serial capture failed: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
