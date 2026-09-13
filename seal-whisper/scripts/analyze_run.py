#!/usr/bin/env python3
"""Validate Seal Whisper CSV runs and summarize suspect samples."""
import argparse, csv, statistics, sys

REQUIRED = {"pressure_pa_s", "ringdown_ms", "result"}
def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("csvfile")
    ap.add_argument("--limit", type=float, default=12.0, help="pressure-decay limit Pa/s")
    a = ap.parse_args()
    with open(a.csvfile, newline="") as f:
        rows = list(csv.DictReader(f))
    if not rows or not REQUIRED.issubset(rows[0]):
        raise SystemExit("CSV must contain pressure_pa_s, ringdown_ms, result")
    leak = [float(r["pressure_pa_s"]) for r in rows]
    tau = [float(r["ringdown_ms"]) for r in rows]
    suspect = [r for r in rows if abs(float(r["pressure_pa_s"])) > a.limit or r["result"] != "PASS"]
    print(f"runs={len(rows)} median_leak_pa_s={statistics.median(leak):.3f} median_ringdown_ms={statistics.median(tau):.3f} suspect={len(suspect)}")
    for r in suspect:
        print(f"suspect: {r.get('utc','unknown')} leak={r['pressure_pa_s']} result={r['result']}")
    return 1 if suspect else 0
if __name__ == "__main__":
    sys.exit(main())
