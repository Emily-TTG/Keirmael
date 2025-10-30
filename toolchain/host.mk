# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

HOST_CC = $(CC)
HOST_AR = $(AR)

HOST_CFLAGS += $(GLOBAL_CFLAGS) $(LOCAL_CFLAGS) $(addprefix -isystem,$(INCLUDES))
HOST_LDFLAGS += $(GLOBAL_LDFLAGS) $(LOCAL_LDFLAGS)

HOST_CFLAGS += -DKML_HOST=1

define host_library
	$(HOST_AR) rc $@ $^
endef

define host_executable
	$(HOST_CC) $(HOST_LDFLAGS) -o $@ $^
endef

%.host.o: %.c
	$(HOST_CC) $(HOST_CFLAGS) -c -o $@ $<
