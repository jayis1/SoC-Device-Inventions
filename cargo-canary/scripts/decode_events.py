#!/usr/bin/env python3
"""Decode Cargo Canary v1 records. Designed by jayis1."""
import argparse
import struct
import sys

RECORD_BYTES = 24

def crc16_ccitt(data: bytes | bytearray) -> int:
    crc = 0xFFFF
    for byte in data:
        crc ^= byte << 8
        for _ in range(8):
            crc = ((crc << 1) ^ 0x1021) & 0xFFFF if crc & 0x8000 else (crc << 1) & 0xFFFF
    return crc

def decode(record: bytes) -> dict:
    if len(record) != RECORD_BYTES or record[:2] != b"CC" or record[2] != 1:
        raise ValueError("not a Cargo Canary v1 record")
    if crc16_ccitt(record[:22]) != struct.unpack_from("<H", record, 22)[0]:
        raise ValueError("CRC mismatch")
    magic, version, flags, seq, utc, mono, temp, shock, battery, crc = struct.unpack("<2sBBIIIhHHH", record)
    return {"sequence": seq, "utc_s": utc, "monotonic_s": mono, "temperature_c": temp / 100, "shock_mg": shock, "battery_mv": battery, "flags": flags, "crc16": f"{crc:04x}"}

def self_test() -> None:
    raw = bytearray(struct.pack("<2sBBIIIhHH", b"CC", 1, 5, 42, 1700000000, 900, -123, 8100, 3550))
    raw += struct.pack("<H", crc16_ccitt(raw))
    item = decode(bytes(raw))
    assert item["sequence"] == 42 and item["temperature_c"] == -1.23 and item["flags"] == 5
    print("self-test: ok")

def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("record_hex", nargs="?", help="one 24-byte record as 48 hex chars")
    parser.add_argument("--self-test", action="store_true")
    args = parser.parse_args()
    if args.self_test: self_test(); return 0
    if not args.record_hex: parser.error("record_hex or --self-test is required")
    print(decode(bytes.fromhex(args.record_hex)))
    return 0
if __name__ == "__main__":
    try: raise SystemExit(main())
    except ValueError as exc: print(f"error: {exc}", file=sys.stderr); raise SystemExit(2)
