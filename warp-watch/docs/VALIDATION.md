# Validation record

Designed and documented by jayis1.

| Check | Result | Evidence |
|---|---|---|
| Host C11 policy build | planned | CMake project supplied; run record is repository-level |
| Policy unit test | planned | `test_warp_watch` checks stable, slack, over-tension, stall, fault, and invalid config |
| Python syntax | planned | `python3 -m py_compile scripts/check_calibration.py` |
| BOM CSV structure | planned | Header and quantity/reference checks required before ordering |
| RP2040/Pico SDK build | not run | Pico SDK and a board definition are not bundled |
| KiCad ERC/DRC | not run | Reference sheet has no resolved library symbols or PCB |
| Electrical, calibration, and loom testing | not performed | Required before relying on readings |

The claimed 0.05–10 N operating range and <=0.1 N repeatability are design targets contingent on the final flexure/load-cell mechanics, two-point calibration, temperature behavior, and test fixture. They are not measured results.
