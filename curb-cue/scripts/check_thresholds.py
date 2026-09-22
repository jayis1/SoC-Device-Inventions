#!/usr/bin/env python3
"""Validate Curb Cue range thresholds without contacting hardware.
Designed and documented by jayis1.
"""
import argparse
import sys


def parse_args():
    parser = argparse.ArgumentParser(description="Validate Curb Cue safe range thresholds.")
    parser.add_argument("--near-mm", type=int, default=750, help="near threshold, 200..3999 mm")
    parser.add_argument("--caution-mm", type=int, default=1500, help="caution threshold, above near and <=4000 mm")
    parser.add_argument("--dry-run", action="store_true", help="validate only; never contact hardware")
    return parser.parse_args()


def main():
    args = parse_args()
    if not 200 <= args.near_mm < args.caution_mm <= 4000:
        print("error: require 200 <= near-mm < caution-mm <= 4000", file=sys.stderr)
        return 2
    print(f"valid: near={args.near_mm} mm caution={args.caution_mm} mm")
    if args.dry_run:
        print("dry-run: no device communication attempted")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
