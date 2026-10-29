#!/usr/bin/env python3
"""Capture Fold Ledger USB CDC JSONL records to a timestamped file.

Usage: python3 scripts/capture.py /dev/ttyACM0 output.jsonl
Requires pyserial: python3 -m pip install pyserial
"""
import sys
from datetime import datetime, timezone


def main() -> int:
    if len(sys.argv) != 3:
        print(__doc__.strip(), file=sys.stderr)
        return 2
    try:
        import serial
    except ImportError:
        print("Missing pyserial; install with: python3 -m pip install pyserial", file=sys.stderr)
        return 2
    port, output = sys.argv[1:]
    with serial.Serial(port, 115200, timeout=1) as device, open(output, "a", encoding="utf-8") as log:
        while True:
            line = device.readline()
            if line is None or len(line) == 0:
                continue
            text = line.decode("utf-8", errors="replace").strip()
            if not text:
                continue
            log.write(text + "\n")
            log.flush()
            print(datetime.now(timezone.utc).isoformat(), text)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
