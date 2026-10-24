#!/usr/bin/env python3
"""Convert rain-gauge USB CDC TIP records into a UTC-relative CSV."""
import argparse
import csv
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input', help='serial capture file (default: stdin)')
    parser.add_argument('--output', help='CSV output (default: stdout)')
    args = parser.parse_args()
    src = open(args.input, encoding='utf-8') if args.input else sys.stdin
    dst = open(args.output, 'w', newline='', encoding='utf-8') if args.output else sys.stdout
    writer = csv.writer(dst)
    writer.writerow(['uptime_ms'])
    for line in src:
        fields = line.strip().split(',')
        if len(fields) == 2 and fields[0] == 'TIP':
            try:
                value = int(fields[1], 10)
            except ValueError:
                continue
            if 0 <= value <= 0xffffffff:
                writer.writerow([value])
    if args.input:
        src.close()
    if args.output:
        dst.close()


if __name__ == '__main__':
    main()
