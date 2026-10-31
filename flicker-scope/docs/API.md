# Serial stream and analysis

Firmware outputs one line per reading: `sample,<time_us>,<adc12>`. `time_us` is Pico boot-relative; ADC values are raw 12-bit readings (0–4095). USB CDC is the transport. Approximate sample interval is 250 µs; timestamps are authoritative because USB printing can cause jitter.

Host tool: `python3 scripts/analyze.py --port PORT --seconds 2`. It collects lines, reports dominant FFT frequency and modulation depth. Install `numpy` and `pyserial` from `scripts/requirements.txt`. The FFT estimate is exploratory; windowing, sensor response, sample jitter, and lighting geometry limit accuracy. No optical calibration is included.
