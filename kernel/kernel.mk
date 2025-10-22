# SPDX-Licence-Identifier: X11
# Copyright (C) 2025 Emily "TTG" Banerjee <prs.ttg+kml@pm.me>

include kernel/vendor/hyper.mk

KERNEL_CFLAGS = -isystem $(HYPER_DIRECTORY)/loader/boot_protocol/ultra_protocol
KERNEL_CFLAGS += -isystem lib/base/include -isystem lib/fat/include

include kernel/arch/arch.mk

kernel/kernel.%.out: LOCAL_CFLAGS = $(KERNEL_CFLAGS)
kernel/kernel.%.out: LOCAL_LDFLAGS = $(KERNEL_LDFLAGS)
kernel/kernel.%.out: $(KERNEL_SOURCES:.c=.%.o) $(KERNEL_LDSCRIPT) lib/libfat.target.a lib/libbase.target.a
	${$*_executable}
