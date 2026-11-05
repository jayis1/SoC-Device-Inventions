# USB CDC event API

Each completed run emits CSV on USB CDC:

`elapsed_ms,sensor,event`

Events: `start` (sensor 0), `arrival` (sensor 1..5), `timeout` (sensor 0 after the 120 s run bound), and `end` (sensor 0). Times are unsigned milliseconds since run start. Rows are emitted in arrival order; a sensor event is accepted after four consecutive low samples at 5 ms intervals. End means all five sensors arrived or timeout occurred. No storage or inbound commands are implemented. Reconnect to start a new host capture; pressing the physical button starts the next run.
