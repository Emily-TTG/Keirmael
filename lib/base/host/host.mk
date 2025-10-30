# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

BASE_HOST_SOURCES = $(wildcard lib/base/host/*.c)

lib/libbase.host.a: INCLUDES += lib/base/host/include
lib/libbase.host.a: $(BASE_HOST_SOURCES:.c=.host.o)

.PHONY: clean_base_host
clean_base_host:
	rm -f $(BASE_HOST_SOURCES:.c=.host.o)
