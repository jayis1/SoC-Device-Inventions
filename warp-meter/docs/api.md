# USB CDC record format

The firmware emits a JSON object followed by LF when a measurement is accepted:

`{"event":"sample","counts":24,"turns":1,"length_mm":100,"turns_per_m":10.0,"yarn_present":true}`

Fields: `counts` is accumulated quadrature detent transitions; `turns` is counts divided by the configured detents-per-turn value; `length_mm` is configured specimen length; `turns_per_m` is derived from turns and length; `yarn_present` is the active optical-gate state. Values are illustrative format only; confirm firmware behavior and calibrate encoder before measurement use.

USB CDC is local and unencrypted. The device accepts no host commands and stores no persistent data. A host may save each line as a record; the firmware does not provide a calibrated uncertainty estimate.