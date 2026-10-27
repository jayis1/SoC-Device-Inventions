# UART API

115200 baud, 8 data bits, no parity, 1 stop bit; 3.3 V logic. Send uppercase or lowercase `T` to arm for up to 100 ms. Close the dry contact to GND to capture up to 20 ms at 10 kHz. Firmware emits `armed`, then header `t_ms,level`, one row per sampled transition, then `summary,sample_count,last_transition_ms`. Levels are 0 (contact asserted) and 1 (idle). There is no network interface or persistent storage.

The timer uses a nominal 48 MHz system clock with a 4,800-count interval; clock setup accuracy has not been verified. Captures are sampled and may miss sub-100 µs pulses. Firmware functions: `setup`, `putchar_uart`, `puts_uart`, `TIM2_IRQHandler`, and `main`.
