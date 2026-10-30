# Firmware core API

`paper_bias.h` defines a streaming accumulator for 12-bit ADC readings and a directional ratio helper.

- `pb_reset(acc)`: clear accumulated sum and sample count.
- `pb_add(acc, adc, &mean)`: accept one reading in [0,4095], update integer mean; returns false for null pointers, out-of-range reading, or saturated sample count.
- `pb_directional_ratio(a,b,deadband,&ratio_milli)`: returns false for invalid pointer or either mean at/below deadband; otherwise writes `max(a,b)/min(a,b)*1000` as an integer. 1000 means equal responses; values >1000 indicate directional difference.

The core neither reads GPIO/ADC nor emits USB records. The eventual adapter should timestamp records with a monotonic clock, reject the invalid state where both orientation switches are active, and emit CSV `orientation,adc_mean,samples`. The physical fixture must define whether increased ADC reading represents increased or decreased deflection before interpreting results.
