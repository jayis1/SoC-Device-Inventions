#!/usr/bin/env python3
"""Compare two Paper Bias mean ADC readings from command-line arguments."""
import argparse


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("orientation_a", type=int, help="12-bit mean ADC for orientation A")
    parser.add_argument("orientation_b", type=int, help="12-bit mean ADC for orientation B")
    parser.add_argument("--deadband", type=int, default=16)
    args = parser.parse_args()
    if any(not 0 <= value <= 4095 for value in (args.orientation_a, args.orientation_b)):
        parser.error("ADC means must be between 0 and 4095")
    if args.deadband < 0:
        parser.error("deadband must be non-negative")
    if min(args.orientation_a, args.orientation_b) <= args.deadband:
        parser.error("both readings must exceed the deadband")
    ratio_milli = max(args.orientation_a, args.orientation_b) * 1000 // min(args.orientation_a, args.orientation_b)
    print(f"ratio={ratio_milli / 1000:.3f}:1; relative only, not calibrated")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
