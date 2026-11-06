#!/usr/bin/env python3
"""Summarize Label Peel CSV records without modifying or transmitting data."""
import csv
import sys


def main(path):
    with open(path, newline="", encoding="utf-8") as stream:
        rows = csv.DictReader(stream)
        if rows.fieldnames != ["ms", "raw", "peak_raw", "event"]:
            raise ValueError("unexpected CSV header")
        values = []
        for row in rows:
            if row["event"] in {"sample", "pressed"}:
                try:
                    values.append((int(row["ms"]), int(row["raw"])))
                except ValueError as exc:
                    raise ValueError("non-integer sample") from exc
    if not values:
        raise ValueError("no valid samples")
    print(f"samples={len(values)} first_ms={values[0][0]} last_ms={values[-1][0]} peak_raw={max(v for _, v in values)}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit(f"usage: {sys.argv[0]} capture.csv")
    try:
        main(sys.argv[1])
    except (OSError, ValueError) as error:
        raise SystemExit(str(error))
