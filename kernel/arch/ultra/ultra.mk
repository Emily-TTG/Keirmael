# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

include kernel/vendor/hyper.mk

KERNEL_ULTRA_SOURCES = $(wildcard kernel/arch/ultra/*.c)

kernel/kernel.target.out: INCLUDES += $(HYPER_DIRECTORY)/loader/boot_protocol/ultra_protocol kernel/arch/ultra/include
kernel/kernel.target.out: $(KERNEL_ULTRA_SOURCES:.c=.target.o)

.PHONY: clean_kernel_ultra
clean_kernel_ultra:
	rm -f $(KERNEL_ULTRA_SOURCES:.c=.target.o)
