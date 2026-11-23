# USB CDC event API

One JSON object per line; USB CDC only, no command parser and no network interface.

- `{"event":"start","count":0}`: session started; elapsed timer begins.
- `{"event":"cycle","count":N,"elapsed_ms":T}`: one accepted active-low Hall edge; N saturates at 4294967295.
- `{"event":"stop","count":N,"elapsed_ms":T}`: session stopped and elapsed time frozen.
- `{"event":"reset","count":0}`: count cleared; session remains stopped.

`elapsed_ms` is monotonic time since session start (Pico timer modulo-safe for ordinary session lengths). Edge qualification rejects edges less than 100 ms after the prior accepted event. Reset buttons are sampled with a 20 ms stable-state debounce. On reboot, state resets to stopped/zero. Host must tolerate device disconnect and incomplete final line.
