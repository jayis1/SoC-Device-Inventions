# Assembly and bring-up

1. Mount the linear potentiometer so its shaft follows the registration fence through its usable range; avoid forcing either end stop.
2. Wire the Pico and passive parts exactly as listed in `../schematic/netlist.md`. Confirm the wiper voltage stays between ground and 3V3 before connecting it to GP26.
3. Connect a data-capable USB cable. Do not attach another supply to the Pico 3V3 rail.
4. Implement Pico SDK ADC0 sampling, enable the GP15 internal pull-up, debounce switch changes in a timer, and pass stable state plus ADC values to the portable event core.
5. On each returned event, send one CSV row on USB CDC. Check the first and last fence positions against known references; retain raw counts for comparisons.

The fixture is a low-voltage prototype. Guard the sliding fence against finger pinching and keep loose wires clear of moving parts. The potentiometer coupling and fence structure are user-designed mechanical components and are not specified as a certified assembly.
