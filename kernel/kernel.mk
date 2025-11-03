# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

KERNEL_INCLUDES += kernel/include kernel/arch/include
KERNEL_INCLUDES += lib/base/include lib/fat/include

include kernel/arch/arch.mk

kernel/kernel.%.out: INCLUDES = $(KERNEL_INCLUDES)
kernel/kernel.%.out: LOCAL_CFLAGS = $(KERNEL_CFLAGS)
kernel/kernel.%.out: LOCAL_LDFLAGS = $(KERNEL_LDFLAGS)
kernel/kernel.%.out: LOCAL_ASFLAGS = $(KERNEL_ASFLAGS)
kernel/kernel.%.out: $(KERNEL_SOURCES:.c=.%.o) $(KERNEL_ASM:.S=.%.o) $(KERNEL_LDSCRIPT) lib/libfat.%.a lib/libbase.%.a
	${$*_executable}

.PHONY: clean_kernel
clean_kernel: clean_hyper
	rm -f $(KERNEL_SOURCES:.c=.host.o)
	rm -f kernel/kernel.host.out

	rm -f $(KERNEL_SOURCES:.c=.target.o)
	rm -f $(KERNEL_ASM:.S=.target.o)
	rm -f kernel/kernel.target.out
