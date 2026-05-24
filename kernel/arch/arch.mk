# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

KERNEL_ARCH_DIRECTORY = kernel/arch/$(TARGET)

KERNEL_SOURCES += $(wildcard $(KERNEL_ARCH_DIRECTORY)/*.c)
KERNEL_ASM += $(wildcard $(KERNEL_ARCH_DIRECTORY)/*.S)

include $(KERNEL_ARCH_DIRECTORY)/$(TARGET).mk

include kernel/arch/host/host.mk

ifeq ($(TARGET),amd64)
	include kernel/arch/ultra/ultra.mk
endif
