#!/usr/bin/env python3
import csv
from pathlib import Path
p=Path(__file__).resolve().parents[1]/'hardware'/'BOM.csv'
with p.open(newline='', encoding='utf-8') as f:
 rows=list(csv.DictReader(f))
assert rows and all(r['Ref'] and r['Qty'] and r['Description'] and r['Manufacturer part'] for r in rows)
print(f'OK: {len(rows)} BOM rows')
