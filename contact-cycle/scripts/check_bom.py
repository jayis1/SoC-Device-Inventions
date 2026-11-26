#!/usr/bin/env python3
import csv
import sys
from pathlib import Path

p = Path(sys.argv[1]) if len(sys.argv) == 2 else Path(__file__).parents[1] / 'hardware' / 'BOM.csv'
with p.open(newline='', encoding='utf-8') as f:
    rows = list(csv.DictReader(f))
required = {'Item', 'Reference', 'Description', 'Qty', 'Footprint/Package', 'Notes'}
if not rows or not required.issubset(rows[0]):
    raise SystemExit('FAIL: missing BOM rows or required columns')
refs = [row['Reference'] for row in rows]
if len(refs) != len(set(refs)):
    raise SystemExit('FAIL: duplicate references')
if any(not row['Item'] or not row['Description'] or not row['Qty'].isdigit() for row in rows):
    raise SystemExit('FAIL: blank item/description or non-integer quantity')
print(f'OK: {len(rows)} BOM rows validated')
