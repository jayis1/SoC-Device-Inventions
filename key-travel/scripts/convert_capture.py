#!/usr/bin/env python3
"""Convert Key Travel raw USB CSV into calibrated force/travel CSV."""
import argparse
import csv
import math
import sys


def convert(rows, pitch_mm, newtons_per_count, zero_angle, zero_force):
    previous = None
    turns = 0.0
    for row in rows:
        try:
            t = int(row["sample_ms"])
            angle = int(row["angle_raw"])
            force = int(row["force_raw"])
            direction = int(row["direction"])
        except (KeyError, ValueError, TypeError):
            continue
        if not 0 <= angle <= 4095 or direction not in (-1, 0, 1):
            continue
        if previous is not None:
            delta = angle - previous
            if delta > 2048:
                delta -= 4096
            elif delta < -2048:
                delta += 4096
            turns += delta / 4096.0
        previous = angle
        travel = (turns - zero_angle / 4096.0) * pitch_mm
        newtons = (force - zero_force) * newtons_per_count
        yield t, f"{travel:.5f}", f"{newtons:.5f}", direction


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("input", nargs="?", default="-", help="serial capture CSV, or - for stdin")
    p.add_argument("--pitch-mm", type=float, default=0.5, help="measured screw lead in mm/revolution")
    p.add_argument("--n-per-count", type=float, required=True, help="calibrated newtons per HX711 count")
    p.add_argument("--zero-angle", type=int, default=0, help="encoder angle at chosen travel zero")
    p.add_argument("--zero-force", type=int, default=0, help="HX711 count at chosen force zero")
    a = p.parse_args()
    if not math.isfinite(a.pitch_mm) or a.pitch_mm <= 0 or not math.isfinite(a.n_per_count):
        p.error("pitch must be positive and calibration finite")
    stream = sys.stdin if a.input == "-" else open(a.input, newline="", encoding="utf-8")
    try:
        reader = csv.DictReader(stream)
        writer = csv.writer(sys.stdout)
        writer.writerow(["sample_ms", "travel_mm", "force_N", "direction"])
        writer.writerows(convert(reader, a.pitch_mm, a.n_per_count, a.zero_angle, a.zero_force))
    finally:
        if stream is not sys.stdin:
            stream.close()


if __name__ == "__main__":
    main()
