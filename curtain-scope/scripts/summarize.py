#!/usr/bin/env python3
"""Summarize a Curtain Scope CSV capture from stdin or a file."""
import argparse
import csv
import sys

def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('file', nargs='?', help='capture CSV; defaults to stdin')
    args = p.parse_args()
    stream = open(args.file, newline='', encoding='utf-8') if args.file else sys.stdin
    rows = list(csv.DictReader(row for row in stream if row.strip() and row.strip() not in ('END',)))
    valid = [r for r in rows if r.get('status') == 'ok']
    if len(valid) != 5:
        p.error(f'expected five ok gate rows; received {len(valid)}')
    times = [int(r['us']) for r in valid]
    print(f'gate_skew_us={max(times)-min(times)}')

if __name__ == '__main__':
    main()
