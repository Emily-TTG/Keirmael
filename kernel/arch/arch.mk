# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

KERNEL_ARCH_DIRECTORY = kernel/arch/$(TARGET)

KERNEL_SOURCES += $(wildcard $(KERNEL_ARCH_DIRECTORY)/*.c)
KERNEL_ASM += $(wildcard $(KERNEL_ARCH_DIRECTORY)/*.S)

include $(KERNEL_ARCH_DIRECTORY)/$(TARGET).mk

KERNEL_LDFLAGS += -T $(KERNEL_LDSCRIPT)

ifeq ($(TARGET),amd64)
	KERNEL_SOURCES += $(wildcard kernel/arch/ultra/*.c)

	include kernel/vendor/hyper.mk
	KERNEL_INCLUDES += $(HYPER_DIRECTORY)/loader/boot_protocol/ultra_protocol

	include kernel/arch/ultra/ultra.mk
endif
