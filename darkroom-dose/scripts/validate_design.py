import csv
import re
from pathlib import Path
base = Path(__file__).resolve().parent.parent
rows = list(csv.DictReader((base / "hardware/BOM.csv").open(newline="")))
assert rows and all(row["Qty"].isdigit() for row in rows)
readme = (base / "README.md").read_text()
links = re.findall(r"`((?:schematic|hardware|firmware|docs|scripts)/[^`]+)`", readme)
missing = [link for link in links if not (base / link).exists()]
assert not missing, missing
print(f"BOM rows={len(rows)}; README links={len(links)}; missing={len(missing)}")
