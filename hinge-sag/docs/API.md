# Host record interface

The target acquisition adapter is not implemented. The intended USB CDC record is one CSV header followed by numeric rows:

`ms,angle_mdeg,delta_mdeg`

- `ms`: monotonic milliseconds from capture start.
- `angle_mdeg`: accelerometer gravity-vector tilt, millidegrees.
- `delta_mdeg`: current angle minus first accepted angle.

Host helper: `python3 scripts/analyze.py capture.csv`. It requires at least two rows, finite values, and strictly increasing timestamps; it prints count, elapsed interval, maximum absolute excursion from the first angle, and final offset. Values are comparative and uncalibrated.
