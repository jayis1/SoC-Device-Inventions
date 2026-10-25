# USB CDC and decoder reference

Firmware prints one JSON object per line:

- `{"clear":N,"red":N,"green":N,"blue":N}`: raw TCS34725 channel counts.
- `{"error":"sensor_not_found","id":N}`: sensor identification/init failure; firmware remains alive.
- `{"error":"i2c_read_failed"}`: a sample transaction failed; subsequent samples continue.

Host helper: `python3 scripts/decode_bands.py <digit1> <digit2> <multiplier> <tolerance>`. Example: `python3 scripts/decode_bands.py brown black red gold` prints the nominal resistance and tolerance. Supported colors: black through white for digits; gold/silver additionally supported as multiplier and tolerance where applicable. The helper does not parse the sensor's raw JSON or infer band colors.
