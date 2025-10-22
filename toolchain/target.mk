# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

TARGET = x86_64

include toolchain/target/$(TARGET).mk

define target_library
	$(TARGET_AR) rc $@ $^
endef

%.target.o: %.c
	$(TARGET_CC) $(GLOBAL_CFLAGS) $(TARGET_CFLAGS) $(LOCAL_CFLAGS) -c -o $@ $<
