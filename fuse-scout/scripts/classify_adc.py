#!/usr/bin/env python3
"""Classify a raw 12-bit RP2040 ADC average for Fuse Scout."""
import argparse


def classify(counts: int) -> str:
    if not 0 <= counts <= 4095:
        raise ValueError("ADC count must be in range 0..4095")
    if counts < 248:
        return "OPEN"
    if counts < 1241:
        return "CHECK"
    return "PASS"


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("adc_counts", type=int)
    print(classify(parser.parse_args().adc_counts))
