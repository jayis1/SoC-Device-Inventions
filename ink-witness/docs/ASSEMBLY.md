# Assembly quick guide

1. Check the BOM and netlist; this is a hand-wired reference, not a PCB design.
2. Confirm sensor breakout tolerates 3.3 V and its DO output is not 5 V.
3. Wire with power disconnected; inspect polarity and continuity.
4. Power Pico from USB through a current-limited supply; verify 3.3 V.
5. Firmware lacks Pico SDK adapter/target build. Run only the host test until implemented.
6. Calibrate the module's threshold with blank paper and a representative ink strip; repeat comparisons with the same geometry.
