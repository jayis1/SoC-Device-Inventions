#!/usr/bin/env python3
"""Generate deterministic Filter Finch algorithm stimuli; no hardware is modeled."""

import argparse
import csv
import random
from collections import Counter
from pathlib import Path

REFERENCE_FLOW_PA = 25.0
CLEAN_DP_PA = 50.0


def classify(filter_pa: float, flow_pa: float) -> tuple[str, float]:
    if filter_pa < -1.0 or filter_pa > 500.0 or flow_pa < -1.0 or flow_pa > 125.0:
        return "TUBE_FAULT", float("nan")
    if flow_pa <= 0.8:
        return "FAN_OFF", float("nan")
    if flow_pa < 4.0:
        return "LOW_FLOW", float("nan")
    if filter_pa < 0.5:
        return "TUBE_FAULT", float("nan")
    ratio = (filter_pa * REFERENCE_FLOW_PA / flow_pa) / CLEAN_DP_PA
    if ratio >= 2.4:
        return "REPLACE", ratio
    if ratio >= 1.8:
        return "WARN_LOADING", ratio
    return "OK", ratio


def generate(seed: int = 7) -> list[dict[str, object]]:
    rng = random.Random(seed)
    rows: list[dict[str, object]] = []
    for index in range(120):
        if index < 20:
            phase, flow_pa, factor = "fan_off", rng.uniform(0.0, 0.6), 1.0
            filter_pa = rng.uniform(-0.2, 0.2)
        elif index < 60:
            phase, flow_pa, factor = "clean", rng.uniform(12.0, 40.0), 1.0
            filter_pa = CLEAN_DP_PA * flow_pa / REFERENCE_FLOW_PA * rng.uniform(0.96, 1.04)
        elif index < 100:
            phase, flow_pa = "loading", rng.uniform(12.0, 40.0)
            factor = 1.0 + 1.7 * (index - 60) / 39.0
            filter_pa = CLEAN_DP_PA * flow_pa / REFERENCE_FLOW_PA * factor * rng.uniform(0.98, 1.02)
        else:
            phase, flow_pa, factor, filter_pa = "reversed_tube", 25.0, 1.0, -3.0
        status, ratio = classify(filter_pa, flow_pa)
        rows.append({
            "sample": index,
            "phase": phase,
            "filter_dp_pa": round(filter_pa, 4),
            "flow_dp_pa": round(flow_pa, 4),
            "loading_factor": round(factor, 4),
            "status": status,
            "computed_ratio": "" if ratio != ratio else round(ratio, 4),
        })
    return rows


def self_test(rows: list[dict[str, object]]) -> None:
    assert len(rows) == 120
    assert all(row["status"] == "FAN_OFF" for row in rows[:20])
    assert all(row["status"] == "OK" for row in rows[20:60])
    loading_states = {row["status"] for row in rows[60:100]}
    assert {"OK", "WARN_LOADING", "REPLACE"}.issubset(loading_states)
    assert all(row["status"] == "TUBE_FAULT" for row in rows[100:])


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, help="optional CSV output path")
    parser.add_argument("--self-test", action="store_true")
    args = parser.parse_args()
    rows = generate()
    if args.self_test:
        self_test(rows)
    if args.output:
        with args.output.open("w", newline="", encoding="utf-8") as handle:
            writer = csv.DictWriter(handle, fieldnames=list(rows[0]))
            writer.writeheader()
            writer.writerows(rows)
    counts = Counter(str(row["status"]) for row in rows)
    print("samples=120 " + " ".join(f"{key}={counts[key]}" for key in sorted(counts)))
    if args.self_test:
        print("self-test: PASS")


if __name__ == "__main__":
    main()
