import csv
from pathlib import Path

path = Path(__file__).parents[1] / "hardware" / "BOM.csv"
with path.open(newline="", encoding="utf-8") as f:
    rows = list(csv.DictReader(f))
required = {"Ref", "Qty", "Value", "Description", "MPN", "Manufacturer", "Notes"}
assert rows and set(rows[0]) == required
assert all(row["Ref"] and int(row["Qty"]) > 0 and row["MPN"] for row in rows)
print(f"BOM syntax OK: {len(rows)} lines; TBD entries require sourcing review")