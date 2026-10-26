# API reference

Transport: USB CDC, ASCII, newline terminated, nominal host setting 115200 baud (USB CDC ignores baud on many hosts).

Events:
- `READY`: sensor is over the calibrated inter-frame gap.
- `ADVANCE`: optical input changed to the image-area state.
- `BUSY`: accepted foot-switch press; action follows.
- `SHOT`: software issued the exposure request; not confirmation of camera exposure.
- `FAULT,film-not-at-gap`: request rejected because sensor indicates image area.

No host commands are implemented. Firmware defaults to shutter output disabled.
