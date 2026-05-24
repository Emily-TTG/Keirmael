# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

TEST_BASE_SOURCES = $(wildcard test/lib/base/*.c)

test/lib/base.%.out: INCLUDES += lib/base/include
test/lib/base.%.out: $(TEST_BASE_SOURCES:.c=.%.o) lib/libbase.%.a
	${$*_executable}

.PHONY: test_base
test_base: test/lib/base.host.out
	$<

.PHONY: clean_test_base
clean_test_base:
	rm -f $(TEST_BASE_SOURCES:.c=.host.o)
	rm -f test/lib/base.host.out

	rm -f $(TEST_BASE_SOURCES:.c=.target.o)
	rm -f test/lib/base.target.out
