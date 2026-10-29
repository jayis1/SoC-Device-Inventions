# USB CDC JSONL protocol

Device emits one JSON object per line; fields are `event`, `count`, and `adc`. `turn` appears once on a qualified low-to-high Hall crossing; `status` appears approximately every five seconds. Example: `{"event":"turn","count":1,"adc":2400}`. Count is volatile and resets on power-up. No commands or network protocol are implemented.
