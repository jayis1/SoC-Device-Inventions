#!/usr/bin/env python3
"""Capture Spine Press Witness USB CDC CSV records (stdlib only)."""
import csv
import sys
import time

try:
    import serial  # pyserial is an optional host dependency
except ImportError:
    serial = None


def main():
    if len(sys.argv) != 3:
        raise SystemExit("usage: capture.py PORT OUTPUT.csv (requires pyserial)")
    if serial is None:
        raise SystemExit("install pyserial: python3 -m pip install pyserial")
    port, output = sys.argv[1:]
    with serial.Serial(port, 115200, timeout=1) as dev, open(output, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["host_unix_s", "device_ms", "raw_counts", "relative_counts"])
        while True:
            line = dev.readline().decode("ascii", errors="replace").strip()
            if not line:
                continue
            fields = line.split(",")
            if len(fields) == 3:
                try:
                    int(fields[0]); int(fields[1]); int(fields[2])
                except ValueError:
                    print("ignored malformed record", file=sys.stderr)
                    continue
                writer.writerow([f"{time.time():.3f}", *fields])
                f.flush()
            elif line == "sensor_error":
                print(line, file=sys.stderr)


if __name__ == "__main__":
    main()
