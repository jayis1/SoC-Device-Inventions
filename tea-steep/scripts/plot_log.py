#!/usr/bin/env python3
"""Plot Tea Steep CSV color channels; requires matplotlib (optional)."""
import argparse
import csv


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("csv_file")
    args = parser.parse_args()
    try:
        import matplotlib.pyplot as plt
    except ImportError as exc:
        raise SystemExit("Install matplotlib to plot: python -m pip install matplotlib") from exc
    rows = list(csv.DictReader(open(args.csv_file, newline="", encoding="utf-8")))
    good = [r for r in rows if r.get("status") == "OK"]
    if not good:
        raise SystemExit("No OK rows; current reference firmware has no temperature driver")
    t0 = int(good[0]["ms"])
    times = [(int(r["ms"]) - t0) / 1000 for r in good]
    for channel in ("r", "g", "b", "clear"):
        plt.plot(times, [int(r[channel]) for r in good], label=channel)
    plt.xlabel("Elapsed time (s)")
    plt.ylabel("Raw sensor counts")
    plt.legend()
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
