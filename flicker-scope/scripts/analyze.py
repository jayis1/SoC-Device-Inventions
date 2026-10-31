#!/usr/bin/env python3
"""Collect Flicker Scope USB samples and print exploratory FFT summary."""
import argparse
import time
import numpy as np
import serial


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--port', required=True, help='USB CDC port, e.g. /dev/ttyACM0')
    parser.add_argument('--seconds', type=float, default=2.0)
    parser.add_argument('--baud', type=int, default=115200)
    args = parser.parse_args()
    if args.seconds <= 0:
        parser.error('--seconds must be positive')
    rows = []
    deadline = time.monotonic() + args.seconds
    with serial.Serial(args.port, args.baud, timeout=0.2) as port:
        while time.monotonic() < deadline:
            try:
                fields = port.readline().decode('ascii', errors='ignore').strip().split(',')
                if len(fields) == 3 and fields[0] == 'sample':
                    rows.append((int(fields[1]), int(fields[2])))
            except ValueError:
                continue
    if len(rows) < 32:
        raise SystemExit(f'only {len(rows)} valid samples; need at least 32')
    t = np.asarray([r[0] for r in rows], dtype=np.float64) * 1e-6
    raw = np.asarray([r[1] for r in rows], dtype=np.float64)
    x = raw - np.mean(raw)
    dt = float(np.median(np.diff(t)))
    if dt <= 0:
        raise SystemExit('invalid timestamps')
    x -= np.mean(x)
    spectrum = np.abs(np.fft.rfft(x * np.hanning(len(x))))
    freq = np.fft.rfftfreq(len(x), dt)
    if len(spectrum) < 2 or np.max(x) == np.min(x):
        raise SystemExit('signal is flat or insufficient')
    peak = int(np.argmax(spectrum[1:]) + 1)
    modulation = 100.0 * (np.max(raw) - np.min(raw)) / (np.max(raw) + np.min(raw) + 1e-12)
    print(f'samples={len(rows)} fs_est={1/dt:.1f} Hz peak={freq[peak]:.2f} Hz modulation={modulation:.1f}%')


if __name__ == '__main__':
    main()
