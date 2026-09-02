#!/usr/bin/env python3
import csv
import math
import sys
from pathlib import Path


def clamp(value, low=0.0, high=1.0):
    return max(low, min(high, value))


def dew_point_c(temp_c, rh):
    a = 17.62
    b = 243.12
    gamma = math.log(max(rh, 1e-3) / 100.0) + (a * temp_c) / (b + temp_c)
    return (b * gamma) / (a - gamma)


def analyze(row):
    ambient_temp_c = float(row['ambient_temp_c'])
    ambient_rh = float(row['ambient_rh'])
    surface_temp_c = float(row['surface_temp_c'])
    thermal_delta_c = abs(float(row['thermal_delta_c']))
    impedance_kohm = float(row['impedance_kohm'])
    phase_deg = abs(float(row['phase_deg']))
    voc_index = float(row['voc_index'])
    gas_resistance_kohm = float(row['gas_resistance_kohm'])

    dp = dew_point_c(ambient_temp_c, ambient_rh)
    dewpoint_margin_c = surface_temp_c - dp
    moisture_proxy = clamp((55.0 - impedance_kohm) / 30.0 + phase_deg / 90.0)
    condensation_risk = clamp((1.5 - dewpoint_margin_c) / 3.5)
    thermal_bridge_score = clamp(thermal_delta_c / 6.0)
    voc_risk = clamp((voc_index - 100.0) / 180.0 + (30.0 - gas_resistance_kohm) / 60.0)
    mold_risk_score = clamp(0.45 * moisture_proxy + 0.30 * condensation_risk + 0.15 * thermal_bridge_score + 0.10 * voc_risk)

    if moisture_proxy > 0.82 and condensation_risk > 0.55:
        state = 'WET_BUILDING_MATERIAL'
    elif mold_risk_score > 0.72:
        state = 'ACTIVE_MOLD_RISK'
    elif moisture_proxy > 0.58:
        state = 'HIDDEN_MOISTURE'
    elif condensation_risk > 0.58:
        state = 'CONDENSATION_RISK'
    else:
        state = 'NORMAL'

    return {
        'timestamp': row['timestamp'],
        'dewpoint_margin_c': dewpoint_margin_c,
        'moisture_proxy': moisture_proxy,
        'condensation_risk': condensation_risk,
        'thermal_bridge_score': thermal_bridge_score,
        'voc_risk': voc_risk,
        'mold_risk_score': mold_risk_score,
        'state': state,
    }


def main(path_str):
    path = Path(path_str)
    with path.open(newline='') as handle:
        rows = list(csv.DictReader(handle))
    if not rows:
        raise SystemExit('no rows found')

    analyzed = [analyze(row) for row in rows]
    worst = max(analyzed, key=lambda row: row['mold_risk_score'])
    avg = sum(row['mold_risk_score'] for row in analyzed) / len(analyzed)

    print(f'rows={len(analyzed)} avg_mold_risk={avg:.3f}')
    for row in analyzed:
        print(
            f"{row['timestamp']},state={row['state']},dew_margin={row['dewpoint_margin_c']:.2f},"
            f"moisture={row['moisture_proxy']:.3f},mold_risk={row['mold_risk_score']:.3f}"
        )
    print(
        f"worst={worst['timestamp']} state={worst['state']} risk={worst['mold_risk_score']:.3f}"
    )


if __name__ == '__main__':
    arg = sys.argv[1] if len(sys.argv) > 1 else str(Path(__file__).with_name('example_scan.csv'))
    main(arg)
