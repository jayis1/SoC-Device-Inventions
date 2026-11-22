# USB CDC API

Device emits newline-terminated JSON at 115200 baud after eight accepted periods:

`{"rpm":33.333,"interval_cv_pct":0.000,"samples":8}`

`rpm` is 60000 divided by mean inter-revolution time in milliseconds. `interval_cv_pct` is population standard deviation divided by mean, in percent; it is a simple interval consistency indicator and not standardized wow/flutter. `samples` is 8. Intervals below 500 ms or above 7000 ms are rejected and reset the current measurement window. No output is emitted until a complete valid window exists. No commands or network interface are implemented.
