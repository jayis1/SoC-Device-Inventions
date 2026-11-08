#!/usr/bin/env python3
"""Plot Cooldown Clerk USB CSV log; ignores # comment records."""
import argparse
import csv
import sys


def load(path):
    xs, ys = [], []
    with open(path, newline="", encoding="utf-8") as f:
        for row in csv.reader(line for line in f if not line.lstrip().startswith("#")):
            if not row or row[0] == "elapsed_s":
                continue
            if len(row) != 2:
                raise ValueError(f"expected 2 columns, got {len(row)}: {row!r}")
            x, y = float(row[0]), float(row[1])
            if x < 0 or not -20 <= y <= 110:
                raise ValueError(f"out-of-range sample: {row!r}")
            xs.append(x); ys.append(y)
    if not xs:
        raise ValueError("no valid measurements found")
    return xs, ys


def main():
    p = argparse.ArgumentParser()
    p.add_argument("csv_file")
    args = p.parse_args()
    try:
        x, y = load(args.csv_file)
        import matplotlib.pyplot as plt
        plt.plot(x, y, marker=".")
        plt.xlabel("Elapsed time (s)"); plt.ylabel("Temperature (°C)")
        plt.grid(True); plt.tight_layout(); plt.show()
    except (OSError, ValueError, ImportError) as e:
        print(f"error: {e}", file=sys.stderr); return 2
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
