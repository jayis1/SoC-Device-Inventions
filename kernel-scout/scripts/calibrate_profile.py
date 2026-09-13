#!/usr/bin/env python3
"""Fit Kernel Scout moisture coefficients from oven-reference CSV data.
CSV columns: magnitude,temperature_c,moisture_wb_pct
"""
import argparse
import csv


def solve_4x4(a, b):
    for pivot in range(4):
        best = max(range(pivot, 4), key=lambda r: abs(a[r][pivot]))
        if abs(a[best][pivot]) < 1e-12:
            raise ValueError("samples do not span enough response values")
        a[pivot], a[best] = a[best], a[pivot]
        b[pivot], b[best] = b[best], b[pivot]
        scale = a[pivot][pivot]
        a[pivot] = [x / scale for x in a[pivot]]
        b[pivot] /= scale
        for row in range(4):
            if row == pivot: continue
            factor = a[row][pivot]
            a[row] = [a[row][col] - factor * a[pivot][col] for col in range(4)]
            b[row] -= factor * b[pivot]
    return b

parser = argparse.ArgumentParser()
parser.add_argument("csv", help="oven-reference records")
parser.add_argument("--baseline", type=float, required=True, help="empty-cup AD5933 magnitude")
args = parser.parse_args()
normal = [[0.0] * 4 for _ in range(4)]
rhs = [0.0] * 4
with open(args.csv, newline="") as f:
    rows = list(csv.DictReader(f))
if len(rows) < 6:
    raise SystemExit("need at least six independent reference samples")
for row in rows:
    response = (float(row["magnitude"]) - args.baseline) / args.baseline
    x = [1.0, response, response * response, float(row["temperature_c"])]
    y = float(row["moisture_wb_pct"])
    for i in range(4):
        rhs[i] += x[i] * y
        for j in range(4): normal[i][j] += x[i] * x[j]
a0, a1, a2, a3 = solve_4x4(normal, rhs)
print(f"a0={a0:.7g}, a1={a1:.7g}, a2={a2:.7g}, a3={a3:.7g}")
