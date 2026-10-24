# USB CDC output

The device emits a JSON object every 500 ms. `matrix[d][s]` is 1 when drive contact `d` registers on sense contact `s`, 0 when open, and -1 when more than one sense channel registers while a drive is active. Multiple paths are ambiguous and should be resolved by checking for shorts or a branched harness. No commands are accepted by firmware.