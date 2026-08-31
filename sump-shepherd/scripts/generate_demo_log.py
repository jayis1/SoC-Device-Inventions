#!/usr/bin/env python3
import csv
from pathlib import Path

ROWS = [
    ["2026-08-31T02:00:00Z", 445, 18, -120, 7.4, 0.26, 72, 1, 0, 4.06, "HEALTHY"],
    ["2026-08-31T02:05:00Z", 520, 22, -4, 0.1, 0.01, 74, 1, 0, 3.98, "NO_START"],
    ["2026-08-31T02:10:00Z", 310, 6, -118, 7.1, 0.23, 73, 0, 0, 4.02, "BACKFLOW"],
    ["2026-08-31T02:15:00Z", 355, 11, -58, 6.8, 0.19, 78, 1, 0, 3.95, "SHORT_CYCLING"],
    ["2026-08-31T02:20:00Z", 118, 0.4, -6, 5.6, 0.38, 69, 0, 0, 3.91, "DRY_RUN"],
    ["2026-08-31T02:25:00Z", 470, 17, -26, 8.2, 0.30, 77, 1, 0, 3.88, "SLOW_PUMP"],
    ["2026-08-31T02:30:00Z", 548, 25, -10, 0.2, 0.02, 89, 1, 1, 3.84, "LEAK_CONFIRMED"],
    ["2026-08-31T02:35:00Z", 250, 1, -2, 0.0, 0.00, 86, 0, 0, 4.10, "HUMIDITY_ALERT"],
]

HEADER = [
    "timestamp",
    "level_mm",
    "rise_rate_mm_min",
    "drawdown_rate_mm_min",
    "pump_current_a",
    "vibration_g_rms",
    "humidity_rh",
    "float_active",
    "leak_active",
    "battery_v",
    "state",
]


def main() -> None:
    out_path = Path(__file__).with_name("generated_demo_log.csv")
    with out_path.open("w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(HEADER)
        writer.writerows(ROWS)
    print(f"wrote {len(ROWS)} rows to {out_path}")


if __name__ == "__main__":
    main()
