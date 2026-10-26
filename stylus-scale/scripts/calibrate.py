#!/usr/bin/env python3
"""Calculate scale zero and counts-per-gram from raw ADC readings."""
import argparse


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--zero", type=int, required=True, help="unloaded raw count")
    parser.add_argument("--loaded", type=int, required=True, help="raw count with calibration mass")
    parser.add_argument("--grams", type=float, required=True, help="known mass in grams")
    args = parser.parse_args()
    if args.grams <= 0 or args.loaded == args.zero:
        parser.error("grams must be positive and loaded count must differ from zero")
    print(f"zero={args.zero}")
    print(f"counts_per_gram={(args.loaded - args.zero) / args.grams:.6f}")


if __name__ == "__main__":
    main()
