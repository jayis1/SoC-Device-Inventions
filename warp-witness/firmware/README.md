# Firmware core

SDK-independent C11 calculation core; no Pico hardware adapter. Pico SDK 2.1.1 target integration is planned but not included. Build-check from repository root:

`cc -std=c11 -Wall -Wextra -Werror -Iwarp-witness/firmware/include -c warp-witness/firmware/src/warp_witness.c -o /tmp/warp_witness.o`

Output values use integer units: `tex_milli` is milli-tex and `turns_per_milli` is milli-turns per metre. Invalid ranges and null output pointers return false without modifying output.
