#!/usr/bin/env python3
import csv
import math
import sys
from collections import defaultdict


def summarize(rows):
    rows = sorted(rows, key=lambda r: float(r["time_ms"]))
    preset = rows[0]["preset"]
    timestamp = rows[0]["timestamp"]
    pull_in_v = math.nan
    operate_ms = math.nan
    bounce_ms = 0.0
    chatter = 0
    max_travel = 0.0
    acoustic_peak = 0.0
    drop_out_v = math.nan
    released = False
    first_close_time = None
    last_toggle_time = None
    stable = None
    bounce_window_end = None
    last_v = 0.0
    last_i = 0.0
    closed_currents = []

    for row in rows:
        v = float(row["coil_v"])
        i = float(row["current_ma"])
        closed = row["contact_closed"] == "1"
        hall = float(row["hall_mm"])
        acoustic = float(row["acoustic"])

        max_travel = max(max_travel, hall)
        acoustic_peak = max(acoustic_peak, acoustic)
        if v > 0.1:
            last_v, last_i = v, i
        if closed:
            closed_currents.append(i)
        if math.isnan(pull_in_v) and (closed or hall >= 0.9):
            pull_in_v = v
            operate_ms = float(row["time_ms"])
            first_close_time = float(row["time_ms"])
            bounce_window_end = first_close_time + 10.0
            stable = closed
            last_toggle_time = first_close_time
            continue
        if first_close_time is not None and float(row["time_ms"]) - first_close_time <= 10.0:
            if closed != stable:
                stable = closed
                chatter += 1
                last_toggle_time = float(row["time_ms"])
        if bounce_window_end is not None and not released and float(row["time_ms"]) >= bounce_window_end and stable and not closed:
            released = True
            drop_out_v = v

    if first_close_time is not None and last_toggle_time is not None and chatter:
        bounce_ms = last_toggle_time - first_close_time

    coil_r = (last_v / (last_i / 1000.0)) if last_i > 1.0 else math.nan
    hold_i = sum(closed_currents) / len(closed_currents) if closed_currents else 0.0

    return {
        "timestamp": timestamp,
        "preset": preset,
        "pull_in_v": pull_in_v,
        "operate_ms": operate_ms,
        "bounce_ms": bounce_ms,
        "chatter": chatter,
        "drop_out_v": drop_out_v,
        "coil_r_ohm": coil_r,
        "hold_current_ma": hold_i,
        "max_travel_mm": max_travel,
        "acoustic_peak": acoustic_peak,
    }


def main(path):
    groups = defaultdict(list)
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            groups[(row["timestamp"], row["preset"])].append(row)

    for _, rows in sorted(groups.items()):
        s = summarize(rows)
        print(
            f"{s['timestamp']} preset={s['preset']} pull_in={s['pull_in_v']:.2f}V "
            f"operate={s['operate_ms']:.2f}ms bounce={s['bounce_ms']:.2f}ms chatter={s['chatter']} "
            f"drop_out={s['drop_out_v']:.2f}V R={s['coil_r_ohm']:.1f}ohm hold={s['hold_current_ma']:.1f}mA "
            f"travel={s['max_travel_mm']:.2f}mm acoustic={s['acoustic_peak']:.2f}"
        )


if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else "demo_capture.csv")
