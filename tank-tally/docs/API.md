# Serial CSV

USB CDC emits `start`, zero or more `event,index,elapsed_ms,interval_ms` lines, then `end,count`. This format is intentionally line-oriented and has no remote protocol. `elapsed_ms` is uptime elapsed from run start; interval is from previous accepted event (the first event is relative to run start). A missing sensor prevents event output; firmware currently has no explicit error line.
