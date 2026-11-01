#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
gcc -std=c11 -Wall -Wextra -Werror -fsyntax-only firmware/main.c
python3 - <<'PY'
import csv
from pathlib import Path
p=Path('hardware/BOM.csv')
rows=list(csv.reader(p.open(newline='')))
assert len(rows)>1 and all(len(r)==len(rows[0]) for r in rows), 'inconsistent BOM columns'
print(f'BOM CSV OK: {len(rows)-1} line items')
PY
