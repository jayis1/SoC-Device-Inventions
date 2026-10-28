#!/usr/bin/env python3
"""Summarize Morse Mate's decoded USB CDC text log from stdin or a file."""
import argparse
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("log", type=Path, help="UTF-8 text log captured from USB CDC")
    args = parser.parse_args()
    text = args.log.read_text(encoding="utf-8", errors="replace")
    letters = [c for c in text.upper() if "A" <= c <= "Z"]
    digits = [c for c in text if c.isdigit()]
    unknown = text.count("?")
    print(f"letters={len(letters)} digits={len(digits)} unknown={unknown}")
    if letters:
        print(f"text={''.join(letters)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
