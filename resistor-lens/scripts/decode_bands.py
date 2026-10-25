#!/usr/bin/env python3
"""Decode four color bands entered as lowercase color names."""
import argparse

DIGITS = {"black": 0, "brown": 1, "red": 2, "orange": 3, "yellow": 4,
          "green": 5, "blue": 6, "violet": 7, "grey": 8, "gray": 8, "white": 9}
MULTIPLIER = {**DIGITS, "gold": -1, "silver": -2}
TOLERANCE = {"brown": 1, "red": 2, "green": 0.5, "blue": 0.25,
             "violet": 0.1, "grey": 0.05, "gold": 5, "silver": 10}

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("bands", nargs=4, help="digit 1, digit 2, multiplier, tolerance colors")
    args = parser.parse_args()
    a, b, mult, tol = (c.lower() for c in args.bands)
    if a not in DIGITS or b not in DIGITS or mult not in MULTIPLIER or tol not in TOLERANCE:
        parser.error("unknown color or invalid 4-band role")
    ohms = (10 * DIGITS[a] + DIGITS[b]) * (10 ** MULTIPLIER[mult])
    print(f"{ohms:g} ohm ±{TOLERANCE[tol]:g}%")

if __name__ == "__main__":
    main()
