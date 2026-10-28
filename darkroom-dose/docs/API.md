# USB CDC interface (current firmware)

At boot, firmware prints `DARKROOM_DOSE,1` and `ready,raw_adc_counts`. On a low-going start-button press (GP6), GPIO8 is asserted and ADC0 is sampled approximately once per millisecond until the button is released or 120 seconds elapse. GPIO8 is then deasserted and one CSV record is printed:

`exposure_ms,<milliseconds>,dose_counts,<sum of 12-bit ADC samples>`

This is uncalibrated integrated ADC count data; the TIA response is unfinished and there is no persisted preset or lux conversion. The mode button and OLED in the product concept are not implemented. Reboot to clear state. On reset or application error the output should remain low; physical fail-safe behavior must be verified on the selected board and optocoupler circuit.
