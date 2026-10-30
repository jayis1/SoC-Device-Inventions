# Copyright (c) jayis1. All rights reserved.
import argparse
import csv
import sys


def estimate(open_v: float, loaded_v: float, load_ohms: float) -> float:
    """Return apparent source resistance in ohms using loaded current estimate."""
    if load_ohms <= 0 or open_v < 0 or loaded_v <= 0 or loaded_v > open_v:
        raise ValueError("require load_ohms > 0 and 0 < loaded_v <= open_v")
    current_a = loaded_v / load_ohms
    return (open_v - loaded_v) / current_a


def main() -> int:
    parser = argparse.ArgumentParser(description="Estimate cell pulse source resistance")
    parser.add_argument("open_v", type=float)
    parser.add_argument("loaded_v", type=float)
    parser.add_argument("--load-ohms", type=float, default=100.0)
    args = parser.parse_args()
    try:
        result = estimate(args.open_v, args.loaded_v, args.load_ohms)
    except ValueError as exc:
        parser.error(str(exc))
    writer = csv.writer(sys.stdout)
    writer.writerow(["approx_source_mOhm"])
    writer.writerow([f"{result * 1000:.1f}"])
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
