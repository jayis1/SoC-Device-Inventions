#!/usr/bin/env python3
"""Capture Flow Ledger JSONL over an explicitly selected serial device."""
import argparse
import json
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--port', required=True, help='serial device path, e.g. /dev/ttyACM0')
    parser.add_argument('--output', required=True, help='output JSONL file')
    args = parser.parse_args()
    try:
        import serial
    except ImportError:
        parser.error('pyserial is required (python3 -m pip install pyserial)')
    try:
        with serial.Serial(args.port, 115200, timeout=1) as dev, open(args.output, 'a', encoding='utf-8') as out:
            while True:
                raw = dev.readline(256)
                if not raw:
                    continue
                try:
                    obj = json.loads(raw.decode('ascii', errors='strict'))
                    if not isinstance(obj, dict) or set(obj) != {'t_ms', 'revolutions'}:
                        continue
                    if any(type(obj[k]) is not int or not 0 <= obj[k] <= 0xffffffff for k in obj):
                        continue
                except (UnicodeDecodeError, json.JSONDecodeError):
                    continue
                out.write(json.dumps(obj, separators=(',', ':')) + '\n')
                out.flush()
                print(obj, file=sys.stderr)
    except KeyboardInterrupt:
        return 0
    except OSError as exc:
        parser.error(str(exc))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
