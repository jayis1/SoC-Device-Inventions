#!/usr/bin/env python3
"""Summarize Hinge Sag CSV (ms,angle_mdeg,delta_mdeg), without changing files."""
import argparse, csv, math

def main():
    p=argparse.ArgumentParser(); p.add_argument('file'); a=p.parse_args()
    rows=[]
    with open(a.file, newline='', encoding='utf-8') as f:
        for n,row in enumerate(csv.DictReader(f),2):
            try: t=float(row['ms']); angle=float(row['angle_mdeg'])
            except (KeyError,ValueError) as e: raise SystemExit(f'row {n}: invalid required columns/data: {e}')
            if not math.isfinite(t) or not math.isfinite(angle): raise SystemExit(f'row {n}: non-finite value')
            rows.append((t,angle))
    if len(rows)<2: raise SystemExit('need at least two valid rows')
    if any(rows[i][0]<=rows[i-1][0] for i in range(1,len(rows))): raise SystemExit('timestamps must increase')
    start=rows[0][1]
    peak=max(abs(x-start) for _,x in rows)
    print(f'samples={len(rows)} elapsed_ms={rows[-1][0]-rows[0][0]:.0f} peak_excursion_mdeg={peak:.0f} final_offset_mdeg={rows[-1][1]-start:.0f}')
if __name__=='__main__': main()
