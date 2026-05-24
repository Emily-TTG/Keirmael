# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

KERNEL_HOST_SOURCES = $(wildcard kernel/arch/host/*.c)

kernel/kernel.host.out: INCLUDES += kernel/arch/host/include lib/base/host/include
kernel/kernel.host.out: $(KERNEL_HOST_SOURCES:.c=.host.o)

.PHONY: clean_kernel_host
clean_kernel_host:
	rm -f $(KERNEL_HOST_SOURCES:.c=.host.o)
