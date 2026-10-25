#!/usr/bin/env python3
"""Print expected LED segments for a duration and elapsed second count."""
import argparse


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("minutes", type=int)
    parser.add_argument("elapsed_seconds", type=int)
    args = parser.parse_args()
    if args.minutes < 1 or args.elapsed_seconds < 0:
        parser.error("minutes must be >= 1 and elapsed_seconds must be >= 0")
    total = min(args.minutes, 120) * 60
    remaining = max(0, total - args.elapsed_seconds)
    count = (remaining * 10 + total - 1) // total if remaining else 0
    print(count)


if __name__ == "__main__":
    main()
