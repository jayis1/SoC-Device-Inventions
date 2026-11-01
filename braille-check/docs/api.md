# Core API and data record

`bc_measure(base_um, top_um, out)` in `firmware/include/measurement.h` accepts exactly six signed displacement pairs, checks null pointers and subtraction overflow, and returns heights, min/max, and integer mean. Return 0 means arithmetic succeeded; -1 means invalid pointer; -2 means a result cannot fit int32.

Planned adapter CSV: `dot,base_um,top_um,height_um` with dot indices 0–5. The host test exercises valid differences and null input. There is no target application, USB record writer, or HX711 acquisition driver in this prototype.
