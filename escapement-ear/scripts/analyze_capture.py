#!/usr/bin/env python3
"""Summarize timestamp intervals from USB CDC CSV: timestamp_us,edge."""
import argparse
import csv
import statistics

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('capture', help='CSV with one unsigned microsecond timestamp per row')
    parser.add_argument('--target-us', type=int, default=500000, help='expected beat period')
    args = parser.parse_args()
    if args.target_us <= 0:
        parser.error('--target-us must be positive')
    stamps=[]
    with open(args.capture, newline='', encoding='utf-8') as f:
        for row in csv.reader(f):
            if not row or row[0].startswith('#'):
                continue
            try:
                value=int(row[0])
            except ValueError:
                if row[0].lower() in ('timestamp_us', 'timestamp'):
                    continue
                raise SystemExit(f'invalid timestamp: {row[0]!r}')
            if not 0 <= value <= 0xffffffff:
                raise SystemExit('timestamp outside uint32 range')
            stamps.append(value)
    intervals=[(b-a) & 0xffffffff for a,b in zip(stamps, stamps[1:])]
    intervals=[x for x in intervals if 80000 <= x < 0x80000000]
    if not intervals:
        raise SystemExit('no valid intervals (need >=2 timestamp rows)')
    median=statistics.median(intervals)
    print(f'valid_intervals={len(intervals)} median_us={median:.1f} rate_s_day={(args.target_us/median-1)*86400:.2f}')

if __name__ == '__main__':
    main()
