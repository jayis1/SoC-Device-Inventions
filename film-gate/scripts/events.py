#!/usr/bin/env python3
"""Read newline-delimited Film Gate event records from a serial port."""
import argparse


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("port", help="serial device, e.g. /dev/ttyACM0")
    parser.add_argument("--baud", type=int, default=115200)
    args = parser.parse_args()
    try:
        import serial
    except ImportError as exc:
        parser.error("install pyserial to use this helper: " + str(exc))
    with serial.Serial(args.port, args.baud, timeout=1) as stream:
        while True:
            record = stream.readline().decode("ascii", errors="replace").strip()
            if record:
                print(record)


if __name__ == "__main__":
    main()
