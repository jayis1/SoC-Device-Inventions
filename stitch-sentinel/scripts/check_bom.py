import csv
from pathlib import Path
p = Path(__file__).resolve().parents[1] / 'hardware' / 'BOM.csv'
with p.open(newline='') as f:
    rows = list(csv.reader(f))
assert rows[0] == ['Ref', 'Qty', 'Value', 'MPN', 'Manufacturer', 'Package', 'Notes']
assert all(len(row) == len(rows[0]) for row in rows)
assert len(rows) > 1
print(f'BOM CSV OK: {len(rows)-1} line items')
