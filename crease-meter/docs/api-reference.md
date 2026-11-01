# Data/API reference

Core function: `bool crease_transmission(uint16_t baseline, uint16_t sample, uint16_t *permille)`. Returns false for null output, baseline below 32 ADC counts, or sample above baseline. Otherwise writes rounded `sample * 1000 / baseline`. Intended ADC width 12 bits (0–4095). Host self-test is enabled with `CREASE_METER_HOST_TEST`. There is no device serial protocol until a target acquisition adapter is implemented.
