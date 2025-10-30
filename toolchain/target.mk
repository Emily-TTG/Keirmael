# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

TARGET = amd64

TARGET_CFLAGS = $(GLOBAL_CFLAGS)

include toolchain/target/$(TARGET).mk

TARGET_CFLAGS += $(GLOBAL_CFLAGS) $(LOCAL_CFLAGS) $(addprefix -isystem ,$(INCLUDES))
TARGET_LDFLAGS += $(GLOBAL_LDFLAGS) $(LOCAL_LDFLAGS)
TARGET_ASFLAGS += $(TARGET_CFLAGS) $(GLOBAL_ASFLAGS) $(LOCAL_ASFLAGS)

TARGET_CFLAGS += -DKML_TARGET=1

define target_library
	$(TARGET_AR) rc $@ $^
endef

define target_executable
	$(TARGET_LD) $(TARGET_LDFLAGS) -o $@ $^
endef

%.target.o: %.c
	$(TARGET_CC) $(TARGET_CFLAGS) -c -o $@ $<

%.target.o: %.S
	$(TARGET_CC) $(TARGET_ASFLAGS) -c -o $@ $<
