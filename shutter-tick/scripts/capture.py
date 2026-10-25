#!/usr/bin/env python3
"""Capture Shutter Tick JSONL from an explicitly selected serial port."""
import argparse
import csv
import json
import time
from pathlib import Path


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--port', required=True, help='explicit USB CDC device')
    p.add_argument('--output', required=True, type=Path)
    p.add_argument('--seconds', type=float, default=30)
    args = p.parse_args()
    try:
        import serial
    except ImportError as exc:
        raise SystemExit('Install pyserial to capture serial data') from exc
    deadline = time.monotonic() + max(0, args.seconds)
    with serial.Serial(args.port, 115200, timeout=0.2) as port, args.output.open('w', newline='') as out:
        writer = csv.writer(out)
        writer.writerow(['host_time_iso', 'duration_us', 'valid', 'error'])
        while time.monotonic() < deadline:
            raw = port.readline(256)
            if not raw:
                continue
            try:
                record = json.loads(raw.decode('utf-8'))
            except (UnicodeDecodeError, json.JSONDecodeError):
                continue
            if not isinstance(record, dict):
                continue
            duration = record.get('duration_us', '')
            if duration != '' and (not isinstance(duration, int) or duration < 0 or duration > 1_000_000):
                continue
            writer.writerow([time.strftime('%Y-%m-%dT%H:%M:%S%z'), duration, record.get('valid', False), record.get('error', '')])


if __name__ == '__main__':
    main()
