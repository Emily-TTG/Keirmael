# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

HOST_CC = $(CC)
HOST_AR = $(AR)

HOST_CFLAGS =

define host_library
	$(AR)$% rc $@ $^
endef

%.host.o: %.c
	$(HOST_CC) $(GLOBAL_CFLAGS) $(HOST_CFLAGS) $(LOCAL_CFLAGS) -c -o $@ $<
