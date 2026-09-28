# Validation record

## Evidence available in this revision

The portable decision core and deterministic synthetic-data harness were executed on a Linux host. They validate software behavior only; they do not validate pressure accuracy, airflow inference, sensor drivers, ESP32 integration, tubing, filter thresholds, or real hardware.

Commands:

```text
cmake -S filter-finch/firmware -B filter-finch/firmware/build
cmake --build filter-finch/firmware/build
ctest --test-dir filter-finch/firmware/build --output-on-failure
./filter-finch/firmware/build/filter_finch_demo
python3 -m py_compile filter-finch/scripts/simulate_filter.py
python3 filter-finch/scripts/simulate_filter.py --self-test --output /tmp/filter-finch-simulation.csv
```

Observed host environment and result for this revision:

```text
GNU C 14.2.0; CMake 3.31.6; Python 3.13.5
CTest: 1/1 test passed; 0 failed
Demo: status=WARN_LOADING normalized_dp=100.00Pa loading_ratio=2.00
Simulation: samples=120 FAN_OFF=20 OK=59 REPLACE=7 TUBE_FAULT=20 WARN_LOADING=14
Simulation self-test: PASS
```

The C tests cover nominal normalization, warn/replace boundaries, fan-off and low-flow handling, reversed-tube and sensor faults, clean-baseline median calibration, and three-sample alarm debounce. The Python self-test covers 120 deterministic synthetic records spanning fan-off, clean operation, increasing loading, and a reversed tube. Generated build products and CSV files are intentionally not committed.

## Hardware validation still required

- Compare both channels at zero and at no fewer than five traceable pressure points over the intended range.
- Traverse the duct with a calibrated instrument; determine whether one pitot point is representative at every fan/damper mode.
- Record clean-filter curves at each normal speed and fit the actual pressure/flow exponent instead of assuming quadratic behavior.
- Load a sacrificial filter in controlled increments and compare the alert ratios with the filter and air-handler manufacturer limits.
- Exercise disconnected, swapped, pinched, wet, and dust-loaded tubes.
- Test brownout, SD removal, sensor timeout, watchdog recovery, and log corruption on target firmware.
- Review installation materials against local mechanical, fire, smoke, and indoor-air requirements.

Until those steps are complete, all performance, threshold, cost, and installation statements are design estimates, not measured product specifications.
