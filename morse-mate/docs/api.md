# USB output protocol

The firmware uses the Pico SDK USB CDC stdio backend. The host sees a serial-like CDC port and receives decoded ASCII characters. Console baud is conventionally opened at 115200; USB CDC itself does not use UART baud timing.

Firmware currently recognizes A–Z and 0–9 Morse patterns. Unsupported or overlong patterns result in `?`. It does not yet emit spaces, accept speed configuration, or expose a command protocol. Output is local to the connected host; no network service is present.
