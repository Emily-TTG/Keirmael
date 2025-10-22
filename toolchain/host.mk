# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

HOST_CC = $(CC)
HOST_AR = $(AR)

define host_library
	$(AR)$% rc $@ $^
endef

%.host.o: %.c
	$(HOST_CC) -c $< -o $@
