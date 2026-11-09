# Text protocol

Host sends one newline-terminated `START <id>` command. IDs must be 1–31 printable ASCII characters excluding quote and backslash. Firmware integration must reject malformed input and bound command buffers. The core produces one JSON object per event:

- `{"type":"start","id":"...","ms":0}`
- `{"type":"mark","id":"...","ms":1234}`
- `{"type":"end","id":"...","ms":5678}`

`ms` is elapsed monotonic milliseconds from START. Unsigned subtraction tolerates timer wrap for sessions shorter than 2^31 ms. Transport framing, command parser, debounce, clock source, and USB CDC driver belong to the yet-to-be-written Pico SDK adapter.
