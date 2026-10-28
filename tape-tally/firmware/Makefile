# Host smoke test; target Pico SDK integration is intentionally not included.
CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror
.PHONY: test
test:
	$(CC) $(CFLAGS) -Isrc src/tape_tally.c test_core.c -o /tmp/tape-tally-test
	/tmp/tape-tally-test
