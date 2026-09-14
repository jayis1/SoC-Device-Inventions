#!/usr/bin/env python3
"""Offline divider-calibration checker for Static Sentinel; no serial or network I/O."""
import argparse
import math


def resistance(v_supply: float, r_reference: float, v_node: float) -> float:
    if not (math.isfinite(v_supply) and math.isfinite(r_reference) and math.isfinite(v_node)):
        raise ValueError("all inputs must be finite")
    if v_supply <= 0 or r_reference <= 0 or not 0 <= v_node < v_supply:
        raise ValueError("require supply/reference > 0 and 0 <= node < supply")
    return r_reference * v_node / (v_supply - v_node)


def main() -> int:
    parser = argparse.ArgumentParser(description="Calculate resistance from an isolated Static Sentinel divider reading.")
    parser.add_argument("--supply", type=float, default=3.3)
    parser.add_argument("--reference-ohms", type=float, default=1_000_000.0)
    parser.add_argument("--node", type=float, required=True)
    args = parser.parse_args()
    print(f"estimated_ohms={resistance(args.supply, args.reference_ohms, args.node):.2f}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
