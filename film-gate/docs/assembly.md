# Assembly

1. Mount the optical module over the film gate; isolate sensor output at 3.3 V.
2. Connect the foot switch between GP15 and ground.
3. Leave GP16 disconnected from any camera until firmware is intentionally built with `FILM_GATE_ENABLE_SHUTTER=1` and the optocoupler circuit is verified.
4. Confirm optocoupler transistor contacts have no continuity to Pico ground.
5. Calibrate the sensor threshold against the exact film and holder, then test with the camera disconnected.

This is a prototype reference. Do not connect a camera cable with unknown pinout or voltage.
