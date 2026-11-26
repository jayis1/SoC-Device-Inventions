# CSV API

Header is `time_ms,closed`. One row is emitted per accepted state transition. `time_ms` is elapsed monotonic milliseconds supplied by the target adapter; `closed` is 1 for conducting and 0 for open. The core rejects backwards timestamps and refuses events after its 256-event fixed buffer fills; callers must surface that overflow rather than presenting a partial capture as complete. No device identity, absolute clock, calibration, or pass/fail judgement is encoded.
