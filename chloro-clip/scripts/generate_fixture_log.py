#!/usr/bin/env python3
import csv
from pathlib import Path

ROWS = [
    {
        "timestamp": "2026-08-26T12:00:00Z",
        "leaf_id": "oak-healthy-01",
        "mode": "dark_adapted",
        "leaf_temp_c": 28.4,
        "ambient_temp_c": 27.1,
        "ambient_par_umol": 780,
        "fo": 118.0,
        "fm": 781.0,
        "fv_fm": 0.849,
        "fs": 428.0,
        "fm_prime": 671.0,
        "fo_prime": 156.0,
        "phi_psii": 0.362,
        "npq": 0.164,
        "qp": 0.472,
        "stress_index": 13.6,
        "diagnosis": "HEALTHY",
    },
    {
        "timestamp": "2026-08-26T12:03:00Z",
        "leaf_id": "pepper-drought-02",
        "mode": "rapid",
        "leaf_temp_c": 34.7,
        "ambient_temp_c": 31.5,
        "ambient_par_umol": 1010,
        "fo": 132.0,
        "fm": 636.0,
        "fv_fm": 0.792,
        "fs": 396.0,
        "fm_prime": 548.0,
        "fo_prime": 168.0,
        "phi_psii": 0.277,
        "npq": 0.161,
        "qp": 0.400,
        "stress_index": 23.8,
        "diagnosis": "MILD_DROUGHT_STRESS",
    },
    {
        "timestamp": "2026-08-26T12:06:00Z",
        "leaf_id": "lettuce-heat-03",
        "mode": "rapid",
        "leaf_temp_c": 39.2,
        "ambient_temp_c": 35.8,
        "ambient_par_umol": 1250,
        "fo": 146.0,
        "fm": 446.0,
        "fv_fm": 0.673,
        "fs": 332.0,
        "fm_prime": 428.0,
        "fo_prime": 184.0,
        "phi_psii": 0.224,
        "npq": 0.042,
        "qp": 0.393,
        "stress_index": 41.4,
        "diagnosis": "HEAT_STRESS",
    },
    {
        "timestamp": "2026-08-26T12:09:00Z",
        "leaf_id": "corn-lowN-04",
        "mode": "light_adapted",
        "leaf_temp_c": 30.1,
        "ambient_temp_c": 29.4,
        "ambient_par_umol": 920,
        "fo": 126.0,
        "fm": 724.0,
        "fv_fm": 0.826,
        "fs": 488.0,
        "fm_prime": 624.0,
        "fo_prime": 164.0,
        "phi_psii": 0.218,
        "npq": 0.160,
        "qp": 0.296,
        "stress_index": 23.3,
        "diagnosis": "NITROGEN_DEFICIENCY",
    },
]


def main() -> None:
    out_path = Path(__file__).resolve().parents[1] / "docs" / "sample-log.csv"
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with out_path.open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=list(ROWS[0].keys()))
        writer.writeheader()
        writer.writerows(ROWS)
    print(f"wrote {out_path}")


if __name__ == "__main__":
    main()
