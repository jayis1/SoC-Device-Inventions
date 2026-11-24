#!/usr/bin/env python3
"""Capture Sky Glow CDC CSV: skyglow_log.py PORT output.csv (requires pyserial)."""
import csv, sys, time

def main():
    if len(sys.argv) != 3:
        raise SystemExit("usage: skyglow_log.py PORT output.csv")
    try:
        import serial
    except ImportError as exc:
        raise SystemExit("install pyserial to capture serial data") from exc
    with serial.Serial(sys.argv[1], 115200, timeout=2) as port, open(sys.argv[2], "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        deadline = time.monotonic() + 3
        while time.monotonic() < deadline:
            if port.readline().decode("ascii", errors="replace").strip() == "ms,ch0,ch1,status": break
        else: raise SystemExit("device CSV header not received")
        writer.writerow(["ms", "ch0", "ch1", "status"])
        while True:
            cols = port.readline().decode("ascii", errors="replace").strip().split(",")
            if len(cols)==4 and all(v.isdigit() for v in cols[:3]): writer.writerow(cols); f.flush()
if __name__ == "__main__": main()
