# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

BASE_SOURCES = $(wildcard lib/base/*.c)

include lib/base/host/host.mk

lib/libbase.%.a: $(BASE_SOURCES:.c=.%.o)
	${$*_library}

# TODO: Define a macro for removing all toolchain variants.
.PHONY: clean_base
clean_base: clean_base_host
	rm -f $(BASE_SOURCES:.c=.host.o)
	rm -f lib/libbase.host.a

	rm -f $(BASE_SOURCES:.c=.target.o)
	rm -f lib/libbase.target.a
