# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

TARGET = amd64

include toolchain/target/$(TARGET).mk

define target_library
	$(TARGET_AR) rc $@ $^
endef

define target_executable
	$(TARGET_LD) $(GLOBAL_LDFLAGS) $(TARGET_LDFLAGS) $(LOCAL_LDFLAGS) -o $@ $^
endef

%.target.o: %.c
	$(TARGET_CC) $(GLOBAL_CFLAGS) $(TARGET_CFLAGS) $(LOCAL_CFLAGS) -c -o $@ $<
