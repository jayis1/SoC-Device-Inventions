# Novelty and design rationale

Job to be done: a hand-loom weaver needs a reliable pick count and interruption record without stopping to make tally marks.

Closest catalog neighbors are Stitch Sentinel (sewing-machine needle/feed mismatch), Row Beacon (knitting row count), and Rain Gauge Keeper (tipping-bucket pulses). Loom Latch observes a hand loom's shuttle traversing two spatially separated optical gates; it classifies order and transit duration and emits directional events. It neither counts a rotating mechanism nor reports environmental pulses. The use case, physical modality, and operating context differ from these entries, although RP2040 overlaps with several.

Distinctive mechanism: ordered dual-beam transit qualification, rejecting same-gate repeats and implausible durations locally before USB output. Candidate alternatives considered: a magnetic shuttle counter (less robust to nearby steel hardware) and a loom motor controller (more invasive and safety-sensitive); the non-contact optical logger was selected.

Status: design reference only, no fabricated hardware, no KiCad source, no verified sensor part number. Validate beam geometry and sensor selection before construction.
