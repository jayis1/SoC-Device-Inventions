#!/usr/bin/env python3
"""Record line-oriented temperature samples from a USB serial adapter."""
import argparse
import csv
import sys
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output", help="CSV output file, or - for stdout")
    parser.add_argument("--seconds", type=float, default=0, help="stop after duration; 0 runs until Ctrl-C")
    args = parser.parse_args()
    try:
        import serial
    except ImportError:
        parser.error("requires pyserial (python -m pip install pyserial)")
    port = input("Serial port (e.g. /dev/ttyACM0): ").strip()
    stream = sys.stdout if args.output == "-" else open(args.output, "w", newline="", encoding="utf-8")
    writer = csv.writer(stream)
    writer.writerow(["host_time_unix_s", "device_line"])
    start = time.monotonic()
    try:
        with serial.Serial(port, 115200, timeout=1) as dev:
            while args.seconds <= 0 or time.monotonic() - start < args.seconds:
                line = dev.readline().decode("ascii", errors="replace").strip()
                if line:
                    writer.writerow([f"{time.time():.3f}", line])
                    stream.flush()
    except KeyboardInterrupt:
        pass
    finally:
        if stream is not sys.stdout:
            stream.close()


if __name__ == "__main__":
    main()
